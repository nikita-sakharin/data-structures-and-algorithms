#ifndef __P_S_H__
#define __P_S_H__

#include <string>

namespace PS
{
    void preprocess(const std::string &, const std::string &, const std::string &);

    template <bool full_output>
    void search(const std::string &, const std::string &, const std::string &);
}

#endif
