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
    each([&](const auto &e) {
      if (pred(e))
        selected.push_back(e);
    });
    return selected;
  }

  bool any(Predicate pred) const {
    bool found = false;
    each([&](const auto &e) { found |= pred(e); });
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
    each([&](const auto &e) {
      if (pred(e))
        ++c;
    });
    return c;
  }

  template <typename F> auto map(F pred) const {
    Container<decltype(pred(T()))> mapped;
    each([&](const auto &e) { mapped.push_back(pred(e)); });
    return mapped;
  }

  template <typename F> auto group_by(F key) const {
    typedef decltype(key(T())) K;
    std::map<K, Container<T>> grouped;

    each([&](const auto &e) { grouped[key(e)].push_back(e); });

    Container<std::pair<K, Container<T>>> x;
    for (const auto &p : grouped) {
      x.push_back(p);
    }

    return x;
  }

  T min() const {
    return min([](const T &t) { return t; });
  }

  T max() const {
    return max([](const T &t) { return t; });
  }

  template <typename F> T min(F key) const {
    typedef std::pair<bool, T> State;
    State initial(true, T());

    return foldl(initial, [key](const auto &t, const auto &v) {
                   return State(
                       false,
                       t.first ? v : ((key(t.second) < key(v)) ? t.second : v));
                 }).second;
  }

  template <typename F> T max(F key) const {
    typedef std::pair<bool, T> State;
    State initial(true, T());

    return foldl(initial, [key](const auto &t, const auto &v) {
                   return State(
                       false,
                       t.first ? v : ((key(v) < key(t.second)) ? t.second : v));
                 }).second;
  }

  template <typename K, typename F> auto foldl(const K &initial, F op) const {
    auto r = initial;
    each([&r, op](const auto &v) { r = op(r, v); });
    return r;
  }
};

#endif
