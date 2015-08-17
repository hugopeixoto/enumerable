#ifndef HUGOPEIXOTO_ENUMERABLE_H_
#define HUGOPEIXOTO_ENUMERABLE_H_

#include <vector>
#include <map>
#include <algorithm>
#include <functional>

template <typename T, template <typename> class Container> class Enumerable {
public:
  typedef std::function<bool(const T &)> Predicate;

  virtual void each(std::function<void(const T &)> callback) const = 0;

  Container<T> select(Predicate pred) const {
    Container<T> selected;
    each([&](auto e) {
      if (pred(e))
        selected.push_back(e);
    });
    return selected;
  }

  bool any(Predicate pred) const {
    bool found = false;
    each([&](auto e) { found |= pred(e); });
    return found;
  }

  bool all(Predicate pred) const {
    return any([&](auto e) { return !pred(e); });
  }

  bool contains(const T &v) const {
    return any([&](auto e) { return e == v; });
  }

  uint64_t count(Predicate pred) const {
    uint64_t c = 0;
    each([&](auto e) {
      if (pred(e))
        ++c;
    });
    return c;
  }

  template <typename T2>
  Container<T2> map(std::function<T2(const T &)> pred) const {
    Container<T2> mapped;
    each([&](auto e) { mapped.push_back(pred(e)); });
    return mapped;
  }

  template <typename K>
  Container<std::pair<K, Container<T>>>
  group_by(std::function<K(const T &)> key) const {
    std::map<K, Container<T>> grouped;

    each([&](auto e) { grouped[key(e)].push_back(e); });

    Container<std::pair<K, Container<T>>> x;
    for (const auto &p : grouped) {
      x.push_back(p);
    }

    return x;
  }

  T min() const {
    return min<const T &>([](const T &t) -> const T & { return t; });
  }

  T max() const {
    return max<const T &>([](const T &t) -> const T & { return t; });
  }

  template <typename K> T min(std::function<K(const T &)> key) const {
    typedef std::pair<bool, T> State;
    State initial(true, T());

    return foldl<State>(initial, [key](const State &t, const T &v) {
                          return State(
                              false,
                              t.first ? v : std::min(key(t.second), key(v)));
                        }).second;
  }

  template <typename K> T max(std::function<K(const T &)> key) const {
    typedef std::pair<bool, T> State;
    State initial(true, T());

    return foldl<State>(initial, [key](const State &t, const T &v) {
                          return State(
                              false,
                              t.first ? v : std::max(key(t.second), key(v)));
                        }).second;
  }

  template <typename K>
  K foldl(const K &initial,
          std::function<K(const K &prev, const T &value)> op) const {
    auto r = initial;
    each([&r, op](auto v) { r = op(r, v); });
    return r;
  }
};

#endif
