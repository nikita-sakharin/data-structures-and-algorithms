#include <cmath>
#include <cstddef>
#include <cstdint>

#include <algorithm>
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
    static Float a_star(const Graph &, Index, Index, Path &, DistanceList &, ParentList &, VisitList &);
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
    DistanceList distance_list(size);
    ParentList parent_list;
    if (full_output)
    {
        parent_list.resize(size);
    }
    VisitList is_black(size);
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
        const Float distance = a_star<full_output>(graph, start, finish, path, distance_list, parent_list, is_black);
        output_stream << distance << '\n';
        if (full_output)
        {
            output_stream << path.size();
            for (auto path_it(path.crbegin()), path_end(path.crend()); path_it != path_end; ++path_it)
            {
                output_stream << ' ' << mapp[*path_it];
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
static PS::Float PS::a_star(const PS::Graph &graph, PS::Index start, PS::Index finish, PS::Path &path, DistanceList &distance_list, ParentList &parent_list, VisitList &is_black)
{
    if (full_output)
    {
        path.clear();
    }
    if (start == finish)
    {
        return 0.l;
    }

    std::fill(distance_list.begin(), distance_list.end(), FLOAT_INFINITY);
    distance_list[start] = 0.l;
    if (full_output)
    {
        parent_list[start] = INDEX_NULL;
    }
    std::fill(is_black.begin(), is_black.end(), false);

    Queue queue;
    queue.emplace(0.l, start);

    Queue::const_iterator it;
    PathSeg path_seg;
    do
    {
        it = queue.begin();
        const Index vertex(it->second);
        if (vertex == finish)
        {
            if (full_output)
            {
                for (Index curr(finish); curr != INDEX_NULL; curr = parent_list[curr])
                {
                    path.push_back(curr);
                }
            }
            return distance_list[finish];
        }
        is_black[vertex] = true;
        queue.erase(it);
        const Float distance(distance_list[vertex]);
        for (const Index &index : graph[vertex].second)
        {
            if (is_black[index])
            {
                continue;
            }
            const Float
                edge_weight(great_circle_distance(graph[vertex].first, graph[index].first)),
                heuristic(great_circle_distance(graph[index].first, graph[finish].first));
            const Float path_length(distance + edge_weight);
            if (path_length < distance_list[index])
            {
                path_seg.second = index;
                if (!std::isinf(distance_list[index]))
                {
                    path_seg.first = distance_list[index] + heuristic;
                    queue.erase(path_seg);
                }
                distance_list[index] = path_length;
                if (full_output)
                {
                    parent_list[index] = vertex;
                }
                path_seg.first = path_length + heuristic;
                queue.insert(path_seg);
            }
        }
    } while (!queue.empty());

    return -1.l;
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
static PS::Float PS::a_star<false>(const PS::Graph &, PS::Index, PS::Index, PS::Path &, DistanceList &, ParentList &, VisitList &);
template
static PS::Float PS::a_star<true>(const PS::Graph &, PS::Index, PS::Index, PS::Path &, DistanceList &, ParentList &, VisitList &);
template
void PS::search<false>(const std::string &, const std::string &, const std::string &);
template
void PS::search<true>(const std::string &, const std::string &, const std::string &);
