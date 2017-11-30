#include <iostream>
#include <string>
#include <vector>

template <typename Count, typename Container>
Count palindrome_count(const Container &);

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::string buffer;
    std::cin >> buffer;
    auto count = palindrome_count<std::uint_least64_t, std::string>(buffer);
    std::cout << count << "\n";
}

template <typename Count, typename Container>
Count palindrome_count(const Container &str)
{
    const std::string::size_type size(str.size());
    if (!size)
    {
        return Count(0u);
    }

    std::vector<std::vector<Count>> d(size, std::vector<Count>(size, Count(0u)));
    for (std::string::size_type i(0u); i < size; ++i)
    {
        d[i][i] = Count(1u);
    }
    for (std::string::size_type l(1u); l < size; ++l)
    {
        for (std::string::size_type i(0u), j(l), count(size - l); i < count; ++i, ++j)
        {
            if (str[i] == str[j])
            {
                d[i][j] = d[i + 1u][j] + d[i][j - 1u] + 1u;
            }
            else
            {
                d[i][j] = d[i + 1u][j] + d[i][j - 1u] - d[i + 1u][j - 1u];
            }
        }
    }

    return d[0u][size - 1u];
}
