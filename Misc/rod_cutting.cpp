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

static constexpr llong MIN_LLONG = numeric_limits<llong>::min();

// rod_cutting - O(n^2)

pair<vector<llong>, vector<llong>> rod_cutting(const vector<llong> &p) noexcept;
void print_solution(ostream &, const vector<llong> &);

int main()
{
    size_t n;
    cin >> n;
    vector<llong> p(n);
    for (llong &p_i : p)
        cin >> p_i;

    const pair<vector<llong>, vector<llong>> r_s = rod_cutting(p);
    print_solution(cout, r_s.second);

    return 0;
}

pair<vector<llong>, vector<llong>>
    rod_cutting(const vector<llong> &p) noexcept
{
    const size_t n = p.size();
    vector<llong> r(n + 1, MIN_LLONG), s(n + 1, MIN_LLONG);
    r[0] = 0;

    for (size_t j = 1; j <= n; ++j)
    {
        for (size_t i = 1; i <= j; ++i)
        {
            if (r[j] < p[i - 1] + r[j - i])
            {
                r[j] = p[i - 1] + r[j - i];
                s[j] = i;
            }
        }
    }

    return make_pair(r, s);
}

void print_solution(ostream &os, const vector<llong> &s)
{
    if (s.size() < 1)
    {
        throw invalid_argument("s.size() < 1");
    }

    size_t n = s.size() - 1;
    while (n)
    {
        os << s[n] << ' ';
        n -= s[n];
    }
    os << '\n';
}
