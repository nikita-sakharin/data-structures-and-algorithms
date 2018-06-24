#ifndef __P_S_H__
#define __P_S_H__

#include <cstdint>

#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace PS
{
    typedef std::uint_least32_t Index;
    typedef double Float;

    typedef std::vector<Index> Path;

    void preprocess(const std::string &, const std::string &, const std::string &);

    template <bool full_output>
    void search(const std::string &, const std::string &, const std::string &);
}

#endif
