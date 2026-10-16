#include <concepts>
#include <cstddef>
#include <random>
#include <utility>

template<
    class Key,
    std::unsigned_integral Priority = uint64_t,
    std::uniform_random_bit_generator Generator = std::mt19937_64
>
class Treap final {
public:
    constexpr Treap() : generatorM{generateSeed(std::random_device{})} {}

    template<class ...Args>
    constexpr Treap(Args&& ...args) : generatorM{std::forward<Args>(args)...} {}

    constexpr ~Treap() noexcept {
        clear();
    }

    constexpr void clear() noexcept {
        auto node{postorderInit(rootM)};
        while (node) {
            const auto nextNode{postorderNext(node)};
            delete node;
            node = nextNode;
        }
    }

    constexpr bool empty() const noexcept {
        return size() == 0;
    }

    constexpr bool insert(const Key &key) {
        auto [parent, link]{find(key)};
        if (*link)
            return false;

        const auto node{
            new Node{key, distributionM(generatorM), nullptr, parent, nullptr}
        };
        *link = node;
        ++sizeM;

        while (parent && node->priority > parent->priority) {
            rotate(node);
            parent = node->parent;
        }

        return true;
    }

    constexpr bool erase(const Key &key) {
        const auto [parent, link]{find(key)};
        const auto node{*link};
        if (!node)
            return false;

        while (true) {
            const auto left{node->left}, right{node->right};
            if (!left || !right)
                break;

            const auto child{left->priority < right->priority
                ? right
                : left
            };
            rotate(child);
        }
        remove(node->parent, getLink(node));

        return true;
    }

    constexpr std::size_t size() const noexcept {
        return sizeM;
    }

private:
    template<std::integral T>
    static constexpr auto width{
        std::numeric_limits<T>::is_signed + std::numeric_limits<T>::digits
    };

    class Node final {
    public:
        Key key{};
        Priority priority{};
        Node *left{}, *parent{}, *right{};
    };

    template<class RandDevice>
    static constexpr Generator::result_type generateSeed(
        RandDevice &&randDevice
    ) noexcept {
        typename Generator::result_type result{0};
        const auto maxStep{width<std::random_device::result_type>};
        auto bits{width<typename Generator::result_type>};
        while (bits > 0) {
            const auto step{std::min(maxStep, bits)};
            result <<= step;
            result |= randDevice();
            bits -= step;
        }

        return result;
    }

    static constexpr bool hasRightSibling(
        const Node * const node
    ) noexcept {
        const auto parent{node->parent};
        return parent && node == parent->left && parent->right;
    }

    static constexpr Node *postorderInit(Node *node) noexcept {
        if (!node)
            return nullptr;

        while (node->left || node->right) {
            if (node->left)
                node = node->left;
            else
                node = node->right;
        }

        return node;
    }

    static constexpr Node *postorderNext(Node *node) noexcept {
        const auto parent{node->parent};
        if (hasRightSibling(node))
            return postorderInit(parent->right);

        return parent;
    }

    Generator generatorM{};
    std::uniform_int_distribution<Priority> distributionM{};
    Node *rootM{};
    std::size_t sizeM{0};

    constexpr std::pair<Node *, Node **> find(const Key &key) {
        Node *node{rootM}, *parent{nullptr}, **link{&rootM};
        while (node) {
            if (node->key < key)
                link = &node->right;
            else if (key < node->key)
                link = &node->left;
            else
                break;

            parent = node;
            node = *link;
        }

        return {parent, link};
    }

    constexpr Node **getLink(Node * const node) noexcept {
        const auto parent{node->parent};
        if (!parent)
            return &rootM;

        if (node == parent->left)
            return &parent->left;

        return &parent->right;
    }

    constexpr void remove(Node *parent, Node **link) {
        const auto node{*link};
        const auto left{node->left}, right{node->right};
        if (!left && !right)
            *link = nullptr;
        else if (left) {
            left->parent = parent;
            *link = left;
        } else {
            right->parent = parent;
            *link = right;
        }

        delete node;
        --sizeM;
    }

    constexpr Node *rotate(Node * const node) noexcept {
        const auto parent{node->parent};
        if (node == parent->left)
            rotateRight(node);
        else
            rotateLeft(node);

        return parent;
    }

    constexpr Node *rotateLeft(Node * const node) noexcept {
        const auto parent{node->parent}, left{node->left};
        *getLink(parent) = node;
        node->parent = parent->parent;

        parent->right = left;
        if (left)
            left->parent = parent;

        node->left = parent;
        parent->parent = node;

        return parent;
    }

    constexpr Node *rotateRight(Node * const node) noexcept {
        const auto parent{node->parent}, right{node->right};
        *getLink(parent) = node;
        node->parent = parent->parent;

        parent->left = right;
        if (right)
            right->parent = parent;

        node->right = parent;
        parent->parent = node;

        return parent;
    }
};
