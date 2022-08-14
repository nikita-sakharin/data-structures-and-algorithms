#include <cstddef>
#include <string>

#include "preprocess_search.hpp"

int main(int argc, const char *argv[])
{
    if (std::string("preprocess") == argv[1])
    {
        std::ptrdiff_t nodes(0), edges(0), output(0);
        std::string nodes_str("--nodes"), edges_str("--edges"), output_str("--output");
        for (std::ptrdiff_t i = 2; i < argc; ++i)
        {
            if (nodes_str == argv[i])
            {
                nodes = ++i;
            }
            else if (edges_str == argv[i])
            {
                edges = ++i;
            }
            else
            {
                output = ++i;
            }
        }
        PS::preprocess(argv[nodes], argv[edges], argv[output]);
    }
    else
    {
        std::ptrdiff_t graph(0), input(0), output(0);
        bool full_output(false);
        std::string graph_str("--graph"), input_str("--input"), output_str("--output"), full_output_str("--full-output");
        for (std::ptrdiff_t i = 2; i < argc; ++i)
        {
            if (graph_str == argv[i])
            {
                graph = ++i;
            }
            else if (input_str == argv[i])
            {
                input = ++i;
            }
            else if (output_str == argv[i])
            {
                output = ++i;
            }
            else if (full_output_str == argv[i])
            {
                full_output = true;
            }
        }
        if (full_output)
        {
            PS::search<true>(argv[graph], argv[input], argv[output]);
        }
        else
        {
            PS::search<false>(argv[graph], argv[input], argv[output]);
        }
    }

    return 0;
}
