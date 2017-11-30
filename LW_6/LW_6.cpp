#include <iostream>

#include "big_integer.hpp"

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout << std::boolalpha;

    BigInteger first, second;
    char op;

    while (true)
    {
        std::cin >> first >> second;
        std::cin >> op;
        if (!std::cin)
        {
            break;
        }
        switch (op)
        {
            case '*':
            {
                std::cout << first * second << "\n";
                break;
            }
            case '+':
            {
                first += second;
                std::cout << first << "\n";
                break;
            }
            case '-':
            {
                try {
                    first -= second;
                    std::cout << first << "\n";
                }
                catch (const std::exception &except)
                {
                    std::cout << "Error\n";
                }
                break;
            }
            case '/':
            {
                try {
                    first /= second;
                    std::cout << first << "\n";
                }
                catch (const std::exception &except)
                {
                    std::cout << "Error\n";
                }
                break;
            }
            case '>':
            {
                std::cout << (first > second) << "\n";
                break;
            }
            case '=':
            {
                std::cout << (first == second) << "\n";
                break;
            }
            case '<':
            {
                std::cout << (first < second) << "\n";
                break;
            }
            case '^':
            {
                try {
                    std::cout << power_eq(first, second) << "\n";
                }
                catch (const std::exception &except)
                {
                    std::cout << "Error\n";
                }
                break;
            }
        }
    }
}
