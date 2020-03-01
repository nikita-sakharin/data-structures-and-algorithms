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

enum Color : int
{
    WHITE,
    GRAY,
    BLACK
};

pair<vector<llong>, vector<llong>> bfs(const vector<vector<ullong>> &,
    ullong);

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

    ullong s;
    cin >> s;

    pair<vector<llong>, vector<llong>> d_pi = bfs(g, s);
    for (ullong i = 0; i < n; ++i)
    {
        cout << "for vertex " << i << ": d = " << d_pi.first[i] << '\n';
    }

    return 0;
}

pair<vector<llong>, vector<llong>> bfs(const vector<vector<ullong>> &g,
    const ullong s)
{
    const ullong n = g.size();
    vector<int> color(n, WHITE);
    vector<llong> d(n, MAX_LLONG), pi(n, -1);
    queue<ullong> q;

    color[s] = GRAY;
    d[s] = 0;
    pi[s] = -1;

    q.push(s);
    while (!q.empty())
    {
        const ullong u = q.front();
        q.pop();
        for (const ullong &v : g[u])
        {
            if (color[v] == WHITE)
            {
                color[v] = GRAY;
                d[v] = d[u] + 1;
                pi[v] = u;
                q.push(v);
            }
        }
        color[u] = BLACK;
    }

    return make_pair(d, pi);
}
