#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

template<typename, typename = void>
struct has_iterator : std::false_type {};

template<typename T>
struct has_iterator<T, void> : std::true_type {};

template<typename, typename = void>
struct has_const_iterator : std::false_type {};

template<typename T>
struct has_const_iterator<T, void> : std::true_type {};

template <typename Container>
struct ContainerTraits {
    static_assert(has_iterator<Container>::value, "Container must support iterators.");
    static_assert(has_const_iterator<Container>::value, "Container must support const_iterators.");
};

template <typename Container, typename Predicate>
auto transformFunc(Container&& container, Predicate&& pred) {
    using ValueType = typename std::decay_t<Container>::value_type;
    std::vector<ValueType> result;
    for (auto& item : container) {
        result.push_back(pred(item));
    }
    return result;
}

template <typename Predicate>
auto transform(Predicate&& pred) {
    return [pred = std::forward<Predicate>(pred)](auto&& container) {
        ContainerTraits<std::decay_t<decltype(container)>>();
        return transformFunc(std::forward<decltype(container)>(container), pred);
    };
}

template <typename Container, typename Predicate>
auto filterFunc(Container&& container, Predicate&& pred) {
    using ValueType = typename std::decay_t<Container>::value_type;
    std::vector<ValueType> result;
    for (auto& item : container) {
        if (pred(item)) {
            result.push_back(item);
        }
    }
    return result;
}

template <typename Predicate>
auto filter(Predicate&& pred) {
    return [pred = std::forward<Predicate>(pred)](auto&& container) {
        ContainerTraits<std::decay_t<decltype(container)>>();
        return filterFunc(std::forward<decltype(container)>(container), pred);
    };
}

template <typename Container>
auto take(Container&& container, std::size_t n) {
    ContainerTraits<std::decay_t<Container>>();
    using ValueType = typename std::decay_t<Container>::value_type;
    std::vector<ValueType> result;
    auto it = std::begin(container);
    for (std::size_t i = 0; i < n && it != std::end(container); ++i, ++it) {
        result.push_back(*it);
    }
    return result;
}

template <typename Container>
auto drop(Container&& container, std::size_t n) {
    ContainerTraits<std::decay_t<Container>>();
    using ValueType = typename std::decay_t<Container>::value_type;
    std::vector<ValueType> result;
    auto it = std::begin(container);
    std::advance(it, n);
    for (; it != std::end(container); ++it) {
        result.push_back(*it);
    }
    return result;
}

template <typename Container>
auto reverse(Container&& container) {
    ContainerTraits<std::decay_t<Container>>();
    using ValueType = typename std::decay_t<Container>::value_type;
    std::vector<ValueType> result(std::rbegin(container), std::rend(container));
    return result;
}

template <typename Map>
auto keys(Map&& map) {
    ContainerTraits<std::decay_t<Map>>();
    using KeyType = typename std::decay_t<Map>::key_type;
    std::vector<KeyType> result;
    for (const auto& pair : map) {
        result.push_back(pair.first);
    }
    return result;
}

template <typename Map>
auto values(Map&& map) {
    ContainerTraits<std::decay_t<Map>>();
    using ValueType = typename std::decay_t<Map>::mapped_type;
    std::vector<ValueType> result;
    for (const auto& pair : map) {
        result.push_back(pair.second);
    }
    return result;
}

template <typename Container, typename Func>
auto operator|(Container&& container, Func&& func) {
    return func(std::forward<Container>(container));
}
