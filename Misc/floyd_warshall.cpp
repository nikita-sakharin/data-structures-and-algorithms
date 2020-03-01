#include <cmath>
#include <cstddef>
#include <cctype>

#include <algorithm>
#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

using namespace std;

template<class T>
using iter = typename T::iterator;
template<class T>
using citer = typename T::const_iterator;
template<class T>
using riter = typename T::reverse_iterator;
template<class T>
using criter = typename T::const_reverse_iterator;

static constexpr dbl INFINITY_DBL = numeric_limits<dbl>::infinity();

pair<vector<vector<dbl>>, vector<vector<llong>>> floyd_warshall(
    const vector<vector<dbl>> &);

int main()
{
    ullong n;
    cin >> n;
    vector<vector<dbl>> g(n);

    for (ullong i = 0; i < n; ++i)
        for (ullong j = 0; j < n; ++j)
            cin >> g[i][j];

    vector<vector<dbl>> d;
    vector<vector<llong>> pi;
    tie(d, pi) = floyd_warshall(g);
}

pair<vector<vector<dbl>>, vector<vector<llong>>> floyd_warshall(
    const vector<vector<dbl>> &g)
{
    const ullong n = g.size();
    vector<vector<dbl>> d = g;
    vector<vector<llong>> pi(n, vector<llong>(n));
    for (ullong i = 0; i < n; ++i)
        for (ullong j = 0; j < n; ++j)
            pi[i][j] = isinf(g[i][j]) ? -1 : i;

    for (ullong k = 0; k < n; ++k)
        for (ullong i = 0; i < n; ++i)
            for (ullong j = 0; j < n; ++j)
                if (d[i][j] > d[i][k] + d[k][j])
                {
                    d[i][j] = d[i][k] + d[k][j];
                    pi[i][j] = pi[k][j];
                }

    return make_pair(d, pi);
}
