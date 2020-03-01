#include <cassert>
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

// longest_common_subsequence - O(n^2)

enum Arrow : int
{
    LEFT = 1,
    UP,
    UP_LEFT
};

pair<vector<vector<int>>, vector<vector<ullong>>>
    longest_common_subsequence(const string &s1, const string &s2) noexcept;
void print_lcs(ostream &, const string &, const vector<vector<int>> &,
    size_t, size_t) noexcept;

int main()
{
    string s1, s2;
    cin >> s1 >> s2;
    const pair<vector<vector<int>>, vector<vector<ullong>>> b_c =
        longest_common_subsequence(s1, s2);
    cout << b_c.second[s1.size()][s2.size()] << '\n';
    print_lcs(cout, s1, b_c.first, s1.size(), s2.size());

    return 0;
}

pair<vector<vector<int>>, vector<vector<ullong>>>
    longest_common_subsequence(const string &s1, const string &s2) noexcept
{
    const size_t m = s1.size(), n = s2.size();
    vector<vector<int>> b(m + 1, vector<int>(n + 1, 0));
    vector<vector<ullong>> c(m + 1, vector<ullong>(n + 1, 0));

    for (size_t i = 1; i <= m; ++i)
    {
        for (size_t j = 1; j <= n; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                c[i][j] = c[i - 1][j - 1] + 1;
                b[i][j] = UP_LEFT;
            }
            else if (c[i - 1][j] >= c[i][j - 1])
            {
                c[i][j] = c[i - 1][j];
                b[i][j] = UP;
            }
            else // if (c[i - 1][j] < c[i][j - 1])
            {
                c[i][j] = c[i][j - 1];
                b[i][j] = LEFT;
            }
        }
    }

    return make_pair(b, c);
}

void print_lcs(ostream &os, const string &s1, const vector<vector<int>> &b,
    size_t i, size_t j) noexcept
{
    if (!i || !j)
    {
        return;
    }

    switch (b[i][j])
    {
        case LEFT:
            print_lcs(os, s1, b, i, --j);
            break;

        case UP:
            print_lcs(os, s1, b, --i, j);
            break;

        case UP_LEFT:
            print_lcs(os, s1, b, --i, --j);
            os << s1[i];
            break;

        default:
            assert(false);
    }
}
