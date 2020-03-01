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

static constexpr uint MAX_UINT = numeric_limits<uint>::max();

// naive algorithm    - O(4^n)
// matrix_chain_order - O(n^3)

pair<vector<vector<ullong>>, vector<vector<ullong>>>
    matrix_chain_order(const vector<uint> &);
void print_optimal_parens(ostream &, const vector<vector<ullong>> &,
    uint, uint) noexcept;

int main()
{
    uint n;
    cin >> n;
    vector<uint> p(n + 1);
    for (uint &p_i : p)
        cin >> p_i;

    const pair<vector<vector<ullong>>, vector<vector<ullong>>> m_s =
        matrix_chain_order(p);
    cout << m_s.first[1][n] << '\n';
    print_optimal_parens(cout, m_s.second, 1, n);

    return 0;
}

pair<vector<vector<ullong>>, vector<vector<ullong>>>
    matrix_chain_order(const vector<uint> &p)
{
    if (p.size() <= 1)
    {
        throw invalid_argument("p.size() <= 1");
    }

    const size_t n = p.size() - 1;
    vector<vector<ullong>> m(n + 1, vector<ullong>(n + 1, 0)),
        s(n, vector<ullong>(n + 1, 0));

    for (size_t l = 2; l <= n; ++l)
    {
        for (size_t i = 1, j = l; i <= n - l + 1; ++i, ++j)
        {
            m[i][j] = MAX_UINT;
            for (size_t k = i; k <= j - 1; ++k)
            {
                const ullong q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (m[i][j] > q)
                {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    return make_pair(m, s);
}

void print_optimal_parens(ostream &os, const vector<vector<ullong>> &s,
    const uint i, const uint j) noexcept
{
    if (i == j)
    {
        os << 'A' << i;
        return;
    }

    os << '(';
    const uint k = s[i][j];
    print_optimal_parens(os, s, i, k);
    print_optimal_parens(os, s, k + 1, j);
    os << ')';
}
