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

static constexpr llong MAX_LLONG = numeric_limits<llong>::max();

enum : uint
{
    WHITE,
    GRAY,
    BLACK
};

tuple<vector<llong>, vector<llong>, vector<llong>>
    dfs(const vector<vector<ullong>> &);
void dfs_visit(const vector<vector<ullong>> &, vector<uint> &, vector<llong> &,
    vector<llong> &, vector<llong> &, llong &, llong) noexcept;

int main()
{
    ullong n, m;
    cin >> n >> m;
    vector<vector<ullong>> g(n);
    for (ullong i = 0; i < m; ++i)
    {
        ullong u, v;
        cin >> u >> v;
        g[u].push_back(v);
    }

    vector<llong> d, f, pi;

    tie(d, f, pi) = dfs(g);
    for (ullong i = 0; i < n; ++i)
    {
        cout << "for vertex " << i <<
            ": d = " << d[i] << ", f = " << f[i] << '\n';
    }

    return 0;
}

tuple<vector<llong>, vector<llong>, vector<llong>>
    dfs(const vector<vector<ullong>> &g)
{
    const ullong n = g.size();
    vector<uint> color(n, WHITE);
    vector<llong> d(n, MAX_LLONG), f(n, MAX_LLONG), pi(n, -1);
    llong dfs_time = 0;

    for (ullong u = 0; u < n; ++u)
        if (color[u] == WHITE)
            dfs_visit(g, color, d, f, pi, dfs_time, u);

    return make_tuple(d, f, pi);
}

void dfs_visit(const vector<vector<ullong>> &g, vector<uint> &color,
    vector<llong> &d, vector<llong> &f, vector<llong> &pi, llong &dfs_time,
    const llong u) noexcept
{
    d[u] = dfs_time++;
    color[u] = GRAY;

    for (const ullong &v : g[u])
        if (color[v] == WHITE)
        {
            pi[v] = u;
            dfs_visit(g, color, d, f, pi, dfs_time, v);
        }

    color[u] = BLACK;
    f[u] = dfs_time++;
}
