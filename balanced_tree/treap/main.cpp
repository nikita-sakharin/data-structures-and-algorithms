#include <cassert>
#include <string>

#include "treap.hpp"

using namespace std;

int main() {
    Treap<std::string> treap{};
    assert(treap.insert("Hello, world!"));
    assert(treap.insert("A+B"));
    assert(treap.insert("3.141592"));
    assert(treap.insert("2.718281"));
    assert(treap.insert("23.140692"));

    assert(!treap.insert("Hello, world!"));
    assert(!treap.insert("A+B"));
    assert(!treap.insert("3.141592"));
    assert(!treap.insert("2.718281"));
    assert(!treap.insert("23.140692"));

    assert(!treap.erase("42"));
    assert(!treap.erase("Foobar"));
    assert(treap.erase("3.141592"));
    assert(treap.erase("2.718281"));
    assert(treap.erase("23.140692"));
    assert(!treap.erase("3.141592"));
    assert(!treap.erase("2.718281"));
    assert(!treap.erase("23.140692"));

    return 0;
}
