#ifndef HUGOPEIXOTO_ENUMERABLE_H_
#define HUGOPEIXOTO_ENUMERABLE_H_

#include <vector>
#include <map>
#include <functional>
#include <hugopeixoto/optional.h>

template <typename T> class EnumerableVector;

template <typename T, template <typename> class Container=EnumerableVector> class Enumerable {
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
    return foldl(false, [&](auto acc, auto e) {
      return acc | pred(e);
    });
  }

  bool all(Predicate pred) const {
    return any([&](auto e) { return !pred(e); });
  }

  bool contains(const T &v) const {
    return any([&](auto e) { return e == v; });
  }

  uint64_t count(Predicate pred) const {
    return foldl(0, [&](auto acc, auto e) {
      return acc + (pred(e) ? 1 : 0);
    });
  }

  template <typename F, typename U> auto map(U (F::*pred)()) const {
    return map([&](auto e){ return (e.*pred)(); });
  }

  template <typename F, typename U> auto map(U (F::*pred)() const) const {
    return map([&](auto e){ return (e.*pred)(); });
  }

  template <typename F, typename U> auto map(U F::*pred) const {
    return map([&](auto e){ return e.*pred; });
  }

  template <typename F> auto map(F pred) const {
    Container<decltype(pred(T()))> mapped;
    each([&](auto e) { mapped.push_back(pred(e)); });
    return mapped;
  }

  template <typename F> auto group_by(F key) const {
    typedef decltype(key(T())) K;
    std::map<K, Container<T>> grouped;

    each([&](auto e) { grouped[key(e)].push_back(e); });

    Container<std::pair<K, Container<T>>> x;
    for (const auto &p : grouped) {
      x.push_back(p);
    }

    return x;
  }

  Optional<T> min() const {
    return min([](auto t) { return t; });
  }

  Optional<T> max() const {
    return max([](auto t) { return t; });
  }

  template <typename F>
  Optional<T> min(F key) const {
    return foldl(Optional<T>(), [key](auto t, auto v) {
      return t.map([&](auto u) {
        return key(u) < key(v) ? u : v;
      }).orElse(v);
    });
  }

  template <typename F> Optional<T> max(F key) const {
    return foldl(Optional<T>(), [key](auto t, auto v) {
      return t.map([&](auto u) {
        return key(v) < key(u) ? u : v;
      }).orElse(v);
    });
  }

  template <typename K, typename F> auto foldl(const K &initial, F op) const {
    auto r = initial;
    each([&r, op](auto v) { r = op(r, v); });
    return r;
  }
};

template <typename T>
class EnumerableVector : public std::vector<T>, public Enumerable<T> {
  virtual void each(std::function<void(const T &)> pred) const override {
    for (const T &e : *this) {
      pred(e);
    }
  }
};

#endif
