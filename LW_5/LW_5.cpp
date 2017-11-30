#include <cstddef>
#include <iostream>
#include <vector>

#include "suffix_tree.h"

int main()
{
    std::ios::sync_with_stdio(false);

    char spec_char = '$';
    std::vector<char> alphabet('z' - 'a' + 1u);
    for (int i = 'a'; i <= 'z'; ++i)
    {
        alphabet[i - 'a'] = i;
    }

    TSuffixTree tree(alphabet, spec_char);

    std::string text;
    std::cin >> text;

    for (std::size_t i = 0u, end = text.size(); i < end; ++i)
    {
        tree.PushBack(text[i]);
    }
    tree.PushBack('$');

    TSuffixArray array(tree);

    for (unsigned long long pattern_counter = 1ull; std::cin >> text; ++pattern_counter)
    {
        std::vector<std::size_t> result = array.Find(text);
        if (!result.empty())
        {
            std::cout << pattern_counter << ": ";
            for (std::size_t i = 0u; i < result.size(); ++i)
            {
                std::cout << result[i] + 1u;
                if (i < result.size() - 1u)
                {
                    std::cout << ", ";
                }
            }
            std::cout << "\n";
        }
    }

    return 0;
}