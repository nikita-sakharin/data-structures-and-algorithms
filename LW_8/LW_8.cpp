#include <cmath>
#include <iostream>
#include <vector>

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::uint_least64_t exp, radix, value;
    std::cin >> exp >> radix >> value;

    std::vector<std::uint_least64_t> result;
    for (auto counter(std::uint_least64_t(std::pow(static_cast<long double>(radix), static_cast<long double>(exp - 1)))); counter; value %= counter, counter /= radix)
    {
        result.push_back(value / counter);
    }
    for (auto it(crbegin(result)), end(crend(result)); it != end; ++it)
    {
        std::cout << *it << "\n";
    }
}
