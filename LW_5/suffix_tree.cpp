#include <algorithm>
#include <limits>
#include <utility>

#include "suffix_tree.h"

TNode::TNode(const std::string &text, TNode * const &node) :
edges(), parent(node), suffixLink(nullptr), begin(text.size() - 1u), length(std::numeric_limits<std::size_t>::max())
{
    node->Edges().insert(std::pair<char, TNode *>(text.back(), this));
}

TNode::TNode(const std::vector<char> &alpha, const char &spec) :
edges(), parent(nullptr), suffixLink(nullptr), begin(0u), length(0u)
{
    suffixLink = new TNode(alpha, spec, this);
}

TNode::TNode(const std::vector<char> &alpha, const char &spec, TNode * const &root) :
edges(), parent(nullptr), suffixLink(nullptr), begin(0u), length(0u)
{
    for (std::vector<char>::const_iterator it = alpha.begin(); it != alpha.end(); ++it)
    {
        edges.insert(std::pair<char, TNode *>(*it, root));
    }
    edges.insert(std::pair<char, TNode *>(spec, root));
}

TNode::TNode(const std::string &text, TNode * const &node, const std::size_t &pos) :
edges({ std::pair<char, TNode *>(text[node->GetBegin() + pos], node) }), parent(node->GetParent()), suffixLink(nullptr), begin(node->GetBegin()), length(pos)
{
    new TNode(text, this);
    node->GetParent()->Edges().at(text[node->GetBegin()]) = this;
    node->SetParent(this);
    node->SetBegin(node->GetBegin() + pos);
    if (node->GetLength() != std::numeric_limits<std::size_t>::max())
    {
        node->SetLength(node->GetLength() - pos);
    }
}

std::size_t TNode::GetBegin() const
{
    return begin;
}

void TNode::SetBegin(const std::size_t &pos)
{
    begin = pos;
}

std::size_t TNode::GetLength() const
{
    return length;
}

void TNode::SetLength(const std::size_t &len)
{
    length = len;
}

TNode *TNode::GetParent() const
{
    return parent;
}

void TNode::SetParent(TNode * const &p)
{
    parent = p;
}

TNode *TNode::GetSuffixLink() const
{
    return suffixLink;
}

void TNode::SetSuffixLink(TNode * const &link)
{
    suffixLink = link;
}

std::map<char, TNode *> &TNode::Edges()
{
    return edges;
}

TNode::~TNode()
{
    for (std::map<char, TNode *>::iterator it = edges.begin(); it != edges.end(); ++it)
    {
        delete it->second;
    }
}

TSuffixTree::TSuffixTree(const std::vector<char> &alpha, const char &spec) :
text(), activeLength(0u), activeCharIdx(0u)
{
    active_vertex = root = new TNode(alpha, spec);
}

void TSuffixTree::PushBack(const char &new_ch)
{
    text.push_back(new_ch);
    std::map<char, TNode *>::iterator it = active_vertex->Edges().end();
    TNode *last_visit = nullptr;
    while ((activeLength < active_vertex->GetLength() && text[active_vertex->GetBegin() + activeLength] != new_ch) || (it = active_vertex->Edges().find(new_ch), activeLength >= active_vertex->GetLength() && it == active_vertex->Edges().end()))
    {
        if (activeLength < active_vertex->GetLength() && text[active_vertex->GetBegin() + activeLength] != new_ch)
        {
            TNode *temp = new TNode(text, active_vertex, activeLength);
            if (last_visit != nullptr)
            {
                last_visit->SetSuffixLink(temp);
            }
            last_visit = active_vertex->GetParent();
            active_vertex = last_visit->GetParent()->GetSuffixLink()->Edges().at(text[activeCharIdx]);
            if (active_vertex == root)
            {
                --activeLength;
                ++activeCharIdx;
            }
            for (std::size_t length; activeLength > active_vertex->GetLength(); activeLength -= length)
            {
                length = active_vertex->GetLength();
                activeCharIdx += length;
                active_vertex = active_vertex->Edges().at(text[activeCharIdx]);
            }
        }
        else
        {
            new TNode(text, active_vertex);
            if (last_visit != nullptr)
            {
                last_visit->SetSuffixLink(active_vertex);
                last_visit = nullptr;
            }
            if (active_vertex != root)
            {
                active_vertex = active_vertex->GetSuffixLink();
                activeLength = active_vertex->GetLength();
            }
        }
    }
    if (last_visit != nullptr)
    {
        last_visit->SetSuffixLink(active_vertex);
    }
    if (activeLength >= active_vertex->GetLength() && it != active_vertex->Edges().end() && it->second->GetBegin() < text.size() - 1u)
    {
        activeLength = 1u;
        active_vertex = it->second;
        activeCharIdx = active_vertex->GetBegin();
    }
    else
    {
        ++activeLength;
    }
}

std::vector<std::size_t> TSuffixTree::Find(const std::string &pattern) const
{
    std::vector<std::size_t> result;
    if (pattern.size() >= text.size())
    {
        return result;
    }

    TNode *curr = root;
    std::size_t summary = 0u;
    for (std::size_t edgeIdx = 0u, pIdx = 0u; pIdx < pattern.size(); ++edgeIdx, ++pIdx)
    {
        if (edgeIdx >= curr->GetLength())
        {
            std::map<char, TNode *>::iterator it = curr->Edges().find(pattern[pIdx]);
            if (it == curr->Edges().end())
            {
                return result;
            }

            curr = it->second;
            if (curr->GetLength() == std::numeric_limits<std::size_t>::max())
            {
                summary += text.size() - curr->GetBegin();
            }
            else
            {
                summary += curr->GetLength();
            }
            edgeIdx = 0u;
        }
        if (pattern[pIdx] != text[curr->GetBegin() + edgeIdx])
        {
            return result;
        }
    }

    DeepFirstSearch(curr, result, summary);

    if (result.size() > 1u)
    {
        std::sort(result.begin(), result.end());
    }

    return result;
}

void TSuffixTree::DeepFirstSearch(TNode * const &curr, std::vector<std::size_t> &result, const std::size_t &summary) const
{
    if (curr->Edges().empty())
    {
        result.push_back(text.size() - summary);

        return;
    }

    for (std::map<char, TNode *>::const_iterator it = curr->Edges().begin(); it != curr->Edges().end(); ++it)
    {
        std::size_t temp = summary;
        if (it->second->GetLength() == std::numeric_limits<std::size_t>::max())
        {
            temp += text.size() - it->second->GetBegin();
        }
        else
        {
            temp += it->second->GetLength();
        }
        DeepFirstSearch(it->second, result, temp);
    }
}

TSuffixTree::~TSuffixTree()
{
    root->GetSuffixLink()->Edges().clear();
    delete root->GetSuffixLink();
    delete root;
}

TSuffixArray::TSuffixArray(const TSuffixTree &tree) :
text(tree.text), array()
{
    tree.DeepFirstSearch(tree.root, array, 0u);
}

std::vector<std::size_t> TSuffixArray::Find(const std::string &pattern) const
{
    std::pair<std::vector<std::size_t>::const_iterator, std::vector<std::size_t>::const_iterator> range(array.begin(), array.end());
    for (std::size_t i = 0u; i < pattern.size() && range.first != range.second; ++i)
    {
        range = equal_range(range.first, range.second, std::numeric_limits<std::size_t>::max(), [this, &pattern, &i] (std::size_t idx1, std::size_t idx2) -> bool {
            if (idx1 == std::numeric_limits<std::size_t>::max())
            {
                return bool(pattern[i] < text[i + idx2]);
            }
            else
            {
                return bool(text[i + idx1] < pattern[i]);
            }
        });
    }
    std::vector<std::size_t> result(range.first, range.second);
    std::sort(result.begin(), result.end());

    return result;
}

TSuffixArray::~TSuffixArray()
{
}