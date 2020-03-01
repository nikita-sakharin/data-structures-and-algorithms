#include <cstddef>

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

typedef signed char schar;
typedef unsigned char uchar;
typedef short shrt;
typedef unsigned short ushrt;
typedef unsigned uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;
typedef float flt;
typedef double dbl;
typedef long double ldbl;

template<class T>
using iter = typename T::iterator;
template<class T>
using citer = typename T::const_iterator;
template<class T>
using riter = typename T::reverse_iterator;
template<class T>
using criter = typename T::const_reverse_iterator;

static constexpr size_t SIZE = 128U, TWO = 2U;

enum Case : size_t
{
    LOWER,
    UPPER
};

// boyer_moore - O(n + m)

int main()
{
    return 0;
}
/*
void BoyerMoore(std::istream &in_stream, std::ostream &out_stream) {
    std::vector<std::string> patern;
    local_result = InputPatern(in_stream, patern);
    if (!local_result) {
        return;
    }
    register const std::size_t pLen = patern.size();

    const std::map<std::string, std::vector<std::size_t>> r = RFunction(patern);
    const std::array<std::vector<std::size_t>, 2u> l = LFunction(patern);

    std::deque<std::pair<std::string, std::array<std::size_t, 2u>>> text;
    for (ptrdiff_t tIdx = n - 1, shift; ; tIdx += shift)
    {
        ptrdiff_t pIdx = n - 1;
        while (pIdx >= 0 && patern[pIdx] == text[tIdx])
            --pIdx, --tIdx;
        if (pIdx < 0)
        {
            out_stream << text.begin()->second[LINE_IDX] + 1u << ", " << text.begin()->second[WORD_IDX] + 1u << "\n";
            shift = pLen - l[LOWER][1u];
        }
        else
        {
            size_t bad_char = BadChar(r, text[tIdx], pIdx),
                good_suff = GoodSuff(l, pIdx);
            shift = std::max(bad_char, good_suff);
        }
    }
}
*/
size_t bad_char(const array<vector<size_t>, SIZE> &r, const char bad,
    const size_t idx) noexcept
{
    citer<vector<size_t>> new_idx =
        upper_bound(r[bad].cbegin(), r[bad].cend(), idx, greater<char>());
    if (new_idx != r[bad].cend()) {
        return idx - *new_idx;
    }

    return idx + 1;
}

array<vector<size_t>, SIZE> r_funcrtion(const string &patern)
{
    array<vector<size_t>, SIZE> res;
    const size_t n = patern.size();
    for (ptrdiff_t i = 0; i < n; ++i) {
        result[patern[i]].push_back(i);
    }

    return result;
}

size_t good_suff(const array<vector<size_t>, TWO> &l, const size_t idx) noexcept
{
    const size_t n = l.front.size();
    if (idx >= n - 1)
    {
        return 1;
    }

    if (l[UPPER][idx + 1])
    {
        return n - 1 - l[UPPER][idx + 1];
    }
    else if (l[LOWER][idx + 1])
    {
        return n - l[LOWER][idx + 1];
    }
    else
    {
        return n;
    }
}

array<vector<size_t>, TWO> l_function(const string &patern)
{
    const size_t n = patern.size();
    vector<size_t> n_upper(n, 0u);
    for (ptrdiff_t i = n - 2, l = n - 1, r = n - 1; i >= 0; --i)
    {
        if (i >= l)
            n_upper[i] = min(i - l + 1, n_upper[n - 1 - r + i]);

        while (i - n_upper[i] >= 0 &&
            patern[n - 1 - n_upper[i]] == patern[i - n_upper[i]])
            ++n_upper[i];

        if (i - n_upper[i] + 1 < l) {
            l = i - n_upper[i] + 1;
            r = i;
        }
    }

    array<vector<size_t>, TWO> res;
    res.fill(vector<size_t>(n, 0u));
    for (size_t i = 0u, max = 0u; i < n - 1; ++i)
    {
        if (n_upper[i])
            res[UPPER][n - n_upper[i]] = i;

        if (n_upper[i] > max && n_upper[i] == i + 1)
            max = n_upper[i];

        res[LOWER][n - 1 - i] = max;
    }

    return res;
}
