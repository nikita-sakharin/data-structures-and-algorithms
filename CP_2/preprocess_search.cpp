#include <cmath>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <array>
#include <fstream>
#include <limits>
#include <set>
#include <utility>
#include <vector>

#include "preprocess_search.hpp"

namespace PS
{
    typedef std::uint_least32_t Index;
    typedef double Float;

    typedef std::pair<Float, Float> Coordin;
    typedef std::vector<Index> Edges;
    typedef std::pair<Coordin, Edges> Vertex;
    typedef std::vector<Vertex> Graph;
    typedef std::vector<Index> Mapp;

    typedef std::vector<Float> DistanceList;
    typedef std::vector<Index> ParentList;
    typedef std::vector<bool> VisitList;
    typedef std::pair<Float, Index> PathSeg;
    typedef std::set<PathSeg> Queue;
    typedef std::vector<Index> Path;

    constexpr static const std::size_t FIRST(0u);
    constexpr static const std::size_t SECOND(1u);
    constexpr static const std::size_t SIZE(2u);

    typedef std::pair<Float, std::array<Index, SIZE>> ShortestPath;
    typedef std::array<std::vector<Float>, SIZE> BinDistanceList;
    typedef std::array<std::vector<Index>, SIZE> BinParentList;
    typedef std::array<std::vector<bool>, SIZE> BinVisitList;
    typedef std::array<std::set<PathSeg>, SIZE> BinQueue;
    typedef std::array<bool, SIZE> Termin;

    constexpr static const Index INDEX_NULL(std::numeric_limits<Index>::max());
    constexpr static const Float FLOAT_INFINITY(std::numeric_limits<Float>::infinity());

    constexpr static const Float
        DEG_TO_RAD(3.1415926535897932384626l / 180.l),
        TWICE_EARTH_RADIUS(6371.0E+3l * 2.l),
        TWO(2.l);

    constexpr static inline Float deg_to_rad(Float);
    constexpr static inline Float great_circle_distance(const Coordin &, const Coordin &);

    constexpr static const std::streamsize PRECISION(6);
    static void preprocess_nodes(const std::string &, Graph &, Mapp &);
    static void preprocess_edges(const std::string &, Graph &, const Mapp &);
    static void preprocess_output(const std::string &, const Graph &, const Mapp &);

    static void search_graph_read(const std::string &, Graph &, Mapp &);

    template <bool full_output>
    static Float bin_a_star(const Graph &, Index, Index, Path &, BinDistanceList &, BinParentList &, BinVisitList &);
    template <bool full_output>
    static void search_init(Index, Index, BinDistanceList &, BinParentList &, BinVisitList &);
    template <bool full_output>
    static void iteration_cycle(const Graph &, Index, Index, BinDistanceList &, BinParentList &, BinVisitList &, ShortestPath &);
    template <bool full_output>
    static void relax(const Graph &, Index, Index, BinDistanceList &, ParentList &, const BinVisitList &, ShortestPath &, Queue &, std::size_t);
    static void find_path(Path &, const BinParentList &, const ShortestPath &);
}

void PS::preprocess(const std::string &nodes, const std::string &edges, const std::string &output)
{
    Graph graph;
    Mapp mapp;

    preprocess_nodes(nodes, graph, mapp);
    preprocess_edges(edges, graph, mapp);
    preprocess_output(output, graph, mapp);
}

template <bool full_output>
void PS::search(const std::string &graph_str, const std::string &input_str, const std::string &output_str)
{
    Graph graph;
    Mapp mapp;

    search_graph_read(graph_str, graph, mapp);
    std::ifstream input_stream(input_str, std::ofstream::in | std::ofstream::binary);
    std::ofstream output_stream(output_str, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
    output_stream.precision(PRECISION);
    output_stream.setf(std::ofstream::fixed, std::ofstream::floatfield);
    auto mapp_begin(mapp.cbegin()), mapp_end(mapp.cend());

    const std::size_t size(graph.size());
    BinDistanceList distance_list;
    distance_list[FIRST].resize(size);
    distance_list[SECOND].resize(size);
    BinParentList parent_list;
    if (full_output)
    {
        parent_list[FIRST].resize(size);
        parent_list[SECOND].resize(size);
    }
    BinVisitList is_black;
    is_black[FIRST].resize(size);
    is_black[SECOND].resize(size);
    Path path;

    for (Index start, finish; !input_stream.eof(); )
    {
        input_stream >> start >> finish;
        if (input_stream.fail())
        {
            break;
        }

        start = std::lower_bound(mapp_begin, mapp_end, start) - mapp_begin;
        finish = std::lower_bound(mapp_begin, mapp_end, finish) - mapp_begin;
        const Float distance = bin_a_star<full_output>(graph, start, finish, path, distance_list, parent_list, is_black);
        output_stream << distance << '\n';
        if (full_output)
        {
            output_stream << path.size();
            for (const Index &index : path)
            {
                output_stream << ' ' << mapp[index];
            }
            output_stream << '\n';
        }
    }
}

static void PS::preprocess_nodes(const std::string &nodes, PS::Graph &graph, PS::Mapp &mapp)
{
    std::ifstream nodes_stream(nodes, std::ifstream::in | std::ifstream::binary);

    Index index;
    Coordin coordin;
    while (!nodes_stream.eof())
    {
        nodes_stream >> index >> coordin.first >> coordin.second;
        if (nodes_stream.fail())
        {
            break;
        }
        coordin.first = deg_to_rad(coordin.first);
        coordin.second = deg_to_rad(coordin.second);
        mapp.push_back(index);
        graph.emplace_back(coordin, Edges());
    }
}

static void PS::preprocess_edges(const std::string &edges, PS::Graph &graph, const PS::Mapp &mapp)
{
    std::ifstream edges_stream(edges, std::ifstream::in | std::ifstream::binary);

    auto mapp_begin(mapp.cbegin()), mapp_end(mapp.cend());
    auto graph_begin(graph.begin());
    Graph::iterator graph_it1, graph_it2;
    for (Index size, index1, index2; !edges_stream.eof(); )
    {
        edges_stream >> size;
        if (edges_stream.fail() || !size)
        {
            break;
        }
        edges_stream >> index1;
        index1 = std::lower_bound(mapp_begin, mapp_end, index1) - mapp_begin;
        graph_it1 = graph_begin + index1;
        while (--size)
        {
            edges_stream >> index2;
            index2 = lower_bound(mapp_begin, mapp_end, index2) - mapp_begin;
            graph_it2 = graph_begin + index2;

            graph_it1->second.emplace_back(index2);
            graph_it2->second.emplace_back(index1);

            graph_it1 = graph_it2;
            index1 = index2;
        }
    }
}

static void PS::preprocess_output(const std::string &output, const PS::Graph &graph, const PS::Mapp &mapp)
{
    std::ofstream output_stream(output, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
    Index size(graph.size());
    output_stream.write(reinterpret_cast<const char *>(&size), sizeof(Index));
    output_stream.write(reinterpret_cast<const char *>(mapp.data()), sizeof(Index) * static_cast<std::size_t>(size));
    for (const Vertex &vertex : graph)
    {
        output_stream.write(reinterpret_cast<const char *>(&vertex.first.first), sizeof(Float));
        output_stream.write(reinterpret_cast<const char *>(&vertex.first.second), sizeof(Float));
        size = vertex.second.size();
        output_stream.write(reinterpret_cast<const char *>(&size), sizeof(Index));
        output_stream.write(reinterpret_cast<const char *>(vertex.second.data()), sizeof(Index) * static_cast<std::size_t>(size));
    }
}

static void PS::search_graph_read(const std::string &graph_str, PS::Graph &graph, PS::Mapp &mapp)
{
    std::ifstream graph_stream(graph_str, std::ifstream::in | std::ifstream::binary);
    Index size;
    graph_stream.read(reinterpret_cast<char *>(&size), sizeof(Index));
    mapp.resize(size);
    graph_stream.read(reinterpret_cast<char *>(mapp.data()), sizeof(Index) * static_cast<std::size_t>(size));
    graph.resize(size);
    for (Vertex &vertex : graph)
    {
        graph_stream.read(reinterpret_cast<char *>(&vertex.first.first), sizeof(Float));
        graph_stream.read(reinterpret_cast<char *>(&vertex.first.second), sizeof(Float));
        graph_stream.read(reinterpret_cast<char *>(&size), sizeof(Index));
        vertex.second.resize(size);
        graph_stream.read(reinterpret_cast<char *>(vertex.second.data()), sizeof(Index) * static_cast<std::size_t>(size));
    }
}

template <bool full_output>
static PS::Float PS::bin_a_star(const PS::Graph &graph, PS::Index start, PS::Index finish, PS::Path &path, PS::BinDistanceList &distance_list, PS::BinParentList &parent_list, PS::BinVisitList &is_black)
{
    if (full_output)
    {
        path.clear();
    }
    if (start == finish)
    {
        return 0.l;
    }

    search_init<full_output>(start, finish, distance_list, parent_list, is_black);
    ShortestPath mu;
    iteration_cycle<full_output>(graph, start, finish, distance_list, parent_list, is_black, mu);
    if (std::isinf(mu.first))
    {
        return -1.l;
    }

    if (full_output)
    {
        find_path(path, parent_list, mu);
    }

    return mu.first;
}

template <bool full_output>
static inline void PS::search_init(PS::Index start, PS::Index finish, PS::BinDistanceList &distance_list, PS::BinParentList &parent_list, PS::BinVisitList &is_black)
{
    std::fill(distance_list[FIRST].begin(), distance_list[FIRST].end(), FLOAT_INFINITY);
    std::fill(distance_list[SECOND].begin(), distance_list[SECOND].end(), FLOAT_INFINITY);
    distance_list[FIRST][start] = 0.l;
    distance_list[SECOND][finish] = 0.l;
    if (full_output)
    {
        parent_list[FIRST][start] = INDEX_NULL;
        parent_list[SECOND][finish] = INDEX_NULL;
    }
    std::fill(is_black[FIRST].begin(), is_black[FIRST].end(), false);
    std::fill(is_black[SECOND].begin(), is_black[SECOND].end(), false);
}

template <bool full_output>
static void PS::iteration_cycle(const PS::Graph &graph, PS::Index start, PS::Index finish, PS::BinDistanceList &distance_list, PS::BinParentList &parent_list, PS::BinVisitList &is_black, PS::ShortestPath &mu)
{
    mu.first = FLOAT_INFINITY;
    BinQueue queue;
    queue[FIRST].emplace(0.l, start);
    queue[SECOND].emplace(0.l, finish);

    Queue::const_iterator it;
    std::size_t turn = FIRST, new_turn(1u);
    Termin termin;
    do
    {
        it = queue[turn].begin();
        const std::size_t another(turn ^ 1u);
        if (mu.first <= it->first)
        {
            termin[turn] = true;
            if (termin[another])
            {
                break;
            }
            turn = another;
            new_turn = 0u;
            continue;
        }
        const Index vertex(it->second);
        is_black[turn][vertex] = true;
        queue[turn].erase(it);
        Index dest(turn ? start : finish);
        relax<full_output>(graph, vertex, dest, distance_list, parent_list[turn], is_black, mu, queue[turn], turn);
        turn ^= new_turn;
    } while (!queue[FIRST].empty() && !queue[SECOND].empty());
}

template <bool full_output>
static void PS::relax(const PS::Graph &graph, PS::Index vertex, PS::Index dest, PS::BinDistanceList &distance_list, PS::ParentList &parent_list, const PS::BinVisitList &is_black, PS::ShortestPath &mu, PS::Queue &queue, std::size_t turn)
{
    PathSeg path_seg;
    const std::size_t another(turn ^ 1u);
    const Float distance(distance_list[turn][vertex]);
    for (const Index &index : graph[vertex].second)
    {
        if (is_black[turn][index])
        {
            continue;
        }
        const Float
            edge_weight(great_circle_distance(graph[vertex].first, graph[index].first)),
            heuristic(great_circle_distance(graph[index].first, graph[dest].first));
        const Float path_length(distance + edge_weight);
        if (path_length < distance_list[turn][index])
        {
            path_seg.second = index;
            if (!std::isinf(distance_list[turn][index]))
            {
                path_seg.first = distance_list[turn][index] + heuristic;
                queue.erase(path_seg);
            }
            distance_list[turn][index] = path_length;
            if (full_output)
            {
                parent_list[index] = vertex;
            }
            path_seg.first = path_length + heuristic;
            queue.insert(path_seg);
            if (is_black[another][index])
            {
                const Float full_distance(path_length + distance_list[another][index]);
                if (full_distance < mu.first)
                {
                    mu.first = full_distance;
                    mu.second[FIRST] = vertex;
                    mu.second[SECOND] = index;
                    if (turn)
                    {
                        std::swap(mu.second[FIRST], mu.second[SECOND]);
                    }
                }
            }
        }
    }
}

static inline void PS::find_path(PS::Path &path, const PS::BinParentList &parent_list, const PS::ShortestPath &mu)
{
    for (std::size_t turn(FIRST); turn < SIZE; ++turn)
    {
        for (Index curr(mu.second[turn]); curr != INDEX_NULL; curr = parent_list[turn][curr])
        {
            path.push_back(curr);
        }
        if (turn == FIRST)
        {
            std::reverse(path.begin(), path.end());
        }
    }
}

constexpr static inline PS::Float PS::deg_to_rad(PS::Float deg)
{
    return deg * DEG_TO_RAD;
}

constexpr static inline PS::Float PS::great_circle_distance(const PS::Coordin &point_1, const PS::Coordin &point_2)
{
    using std::asin; using std::cos; using std::sin;
    using std::pow; using std::sqrt;
    using std::fabs;
    using std::fma;

    register const Float half_d_phi(fabs(point_1.first - point_2.first) / TWO), half_d_lambda(fabs(point_1.second - point_2.second) / TWO);
    register const Float d_sigma(fma(pow(sin(half_d_lambda), TWO), cos(point_1.first) * cos(point_2.first), pow(sin(half_d_phi), TWO)));

    return asin(sqrt(d_sigma)) * TWICE_EARTH_RADIUS;
}

template
void PS::search<false>(const std::string &, const std::string &, const std::string &);
template
void PS::search<true>(const std::string &, const std::string &, const std::string &);
template
static PS::Float PS::bin_a_star<false>(const PS::Graph &, PS::Index, PS::Index, PS::Path &, PS::BinDistanceList &, PS::BinParentList &, PS::BinVisitList &);
template
static PS::Float PS::bin_a_star<true>(const PS::Graph &, PS::Index, PS::Index, PS::Path &, PS::BinDistanceList &, PS::BinParentList &, PS::BinVisitList &);
template
static void PS::search_init<false>(PS::Index, PS::Index, PS::BinDistanceList &, PS::BinParentList &, PS::BinVisitList &);
template
static void PS::search_init<true>(PS::Index, PS::Index, PS::BinDistanceList &, PS::BinParentList &, PS::BinVisitList &);
template
static void PS::iteration_cycle<false>(const PS::Graph &, PS::Index, PS::Index, PS::BinDistanceList &, PS::BinParentList &, PS::BinVisitList &, PS::ShortestPath &);
template
static void PS::iteration_cycle<true>(const PS::Graph &, PS::Index, PS::Index, PS::BinDistanceList &, PS::BinParentList &, PS::BinVisitList &, PS::ShortestPath &);
template
static void PS::relax<false>(const PS::Graph &, PS::Index, PS::Index, PS::BinDistanceList &, PS::ParentList &, const PS::BinVisitList &, PS::ShortestPath &, PS::Queue &, std::size_t);
template
static void PS::relax<true>(const PS::Graph &, PS::Index, PS::Index, PS::BinDistanceList &, PS::ParentList &, const PS::BinVisitList &, PS::ShortestPath &, PS::Queue &, std::size_t);
