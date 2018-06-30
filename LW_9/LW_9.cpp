#include <cstdint>
#include <iostream>
#include <vector>
#include <limits>

typedef std::uint_least32_t Index;
constexpr static const Index INDEX_NULL(std::numeric_limits<Index>::max()); // (.pi = NIL) <=> (.second = INDEX_NULL)
typedef std::int_least32_t Weight;
typedef std::int_least64_t PathLength;
constexpr static const PathLength PATH_LENGTH_INF(std::numeric_limits<PathLength>::max()); // (.d = inf) <=> (.first = PATH_LENGTH_INF)
typedef std::pair<Index, Weight> Edge;
typedef std::vector<Edge> Vertex; // in any order
typedef std::vector<Vertex> Graph;
typedef std::pair<PathLength, Index> Destination; //.d == .first, .pi == .second
typedef std::vector<Destination> PathMap;

PathMap bellman_ford(const Graph &, const Index &);

int main()
{
    std::ios_base::sync_with_stdio(false);
    Index n, m, start, finish;
    std::cin >> n >> m >> start >> finish;
    --start;
    --finish;
    Graph graph(n);

    for (register Index i(0u); i < m; ++i)
    {
        Index u, v;
        Weight w;
        std::cin >> u >> v >> w;
        --u;
        --v;
        graph[u].emplace_back(v, w);
    }

    PathLength result(bellman_ford(graph, start)[finish].first);

    if (result != PATH_LENGTH_INF)
    {
        std::cout << result << "\n";
    }
    else
    {
        std::cout << "No solution\n";
    }
}

static inline bool relax(const Index &, const Edge &, PathMap &);

PathMap bellman_ford(const Graph &graph, const Index &start)
{
    PathMap result(graph.size(), Destination(PATH_LENGTH_INF, INDEX_NULL));
    result[start].first = 0u;

    for (register std::size_t i(1u), size(graph.size()), j; i < size; ++i)
    {
        j = 0u;
        bool is_changed(false);
        for (const Vertex &vertex : graph)
        {
            for (const Edge &edge : vertex)
            {
                is_changed |= relax(j, edge, result);
            }
            ++j;
        }
        if (!is_changed)
        {
            break;
        }
    }

    return result;
}

static inline bool relax(const Index &u, const Edge &u_v, PathMap &path_map)
{
    if (path_map[u].first == PATH_LENGTH_INF)
    {
        return false;
    }

    PathLength length(path_map[u].first + static_cast<PathLength>(u_v.second));
    if (path_map[u_v.first].first > length)
    {
        path_map[u_v.first].first = length;
        path_map[u_v.first].second = u;
        return true;
    }

    return false;
}
