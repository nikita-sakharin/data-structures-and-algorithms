#include <cstddef>
#include <map>
#include <vector>
#include <string>

class TNode
{
public:
    std::map<char, TNode *> edges;
    TNode *parent;
    TNode *suffixLink;
    std::size_t begin;
    std::size_t length;

    TNode();
    TNode(const std::string &, TNode * const &);
    TNode(const std::vector<char> &, const char &);
    TNode(const std::vector<char> &, const char &, TNode * const &);
    TNode(const std::string &, TNode * const &, const std::size_t &);

    std::size_t GetBegin() const;
    void SetBegin(const std::size_t &);
    std::size_t GetLength() const;
    void SetLength(const std::size_t &);
    TNode *GetParent() const;
    void SetParent(TNode * const &);
    TNode *GetSuffixLink() const;
    void SetSuffixLink(TNode * const &);

    std::map<char, TNode *> &Edges();

    ~TNode();
};

class TSuffixTree;
class TSuffixArray;

class TSuffixArray
{
    std::string text;
    std::vector<std::size_t> array;

public:
    TSuffixArray(const TSuffixTree &);

    std::vector<std::size_t> Find(const std::string &pattern) const;

    ~TSuffixArray();
};

class TSuffixTree
{
    std::string text;
    TNode *root;
    TNode *active_vertex;
    std::size_t activeLength;
    std::size_t activeCharIdx;

    void DeepFirstSearch(TNode * const &, std::vector<std::size_t> &, const std::size_t &) const;
    friend TSuffixArray::TSuffixArray(const TSuffixTree &);

public:
    TSuffixTree(const std::vector<char> &, const char &);

    void PushBack(const char &ch);
    std::vector<std::size_t> Find(const std::string &pattern) const;

    ~TSuffixTree();
};