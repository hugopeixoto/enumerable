#ifndef HUGOPEIXOTO_ENUMERABLE_H_
#define HUGOPEIXOTO_ENUMERABLE_H_

#include <vector>
#include <map>
#include <functional>
#include <optional>

namespace enumerable {
  namespace select {
    template<typename I, typename T>
      class iterator {
      public:
        typedef std::function<bool(const T &)> Predicate;

        iterator(const iterator& ci) :
          current(ci.current), end(ci.end), fn(ci.fn) {}

        iterator(const I& current, const I& end, Predicate fn) :
          current(current), end(end), fn(fn) {}

        T operator*() const { return *current; }

        iterator& operator++() {
          do {
            ++this->current;
          } while (this->current != this->end && !fn(*this->current));
          return *this;
        }

        bool operator==(const iterator& other) const {
          return current == other.current;
        }
        bool operator!=(const iterator& other) const {
          return !(*this == other);
        }

        // using difference_type = ???
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::forward_iterator_tag;

      private:
        I current;
        I end;
        Predicate fn;
      };
  }

  namespace map {
    template<typename I, typename From, typename To>
      class iterator {
      public:
        typedef std::function<To(const From&)> Predicate;

        iterator(const iterator& ci) :
          current(ci.current), end(ci.end), fn(ci.fn) {}

        iterator(const I& current, const I& end, Predicate fn) :
          current(current), end(end), fn(fn) {}

        To operator*() const { return fn(*current); }

        iterator& operator++() { ++this->current; return *this; }
        iterator& operator++(int) { iterator ret = *this; ++(*this); return ret; }

        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

        // using difference_type = ???
        using value_type = To;
        using pointer = const To*;
        using reference = const To&;
        using iterator_category = std::forward_iterator_tag;

      private:
        I current;
        I end;
        Predicate fn;
      };
  }
}

template<typename I, typename T = typename I::value_type> class Enumerable {
public:
  typedef std::function<bool(const T &)> Predicate;

  Enumerable(I begin, I end) :
    begin_it(begin), end_it(end) {}

  Enumerable(const Enumerable<I, T>& i) :
    begin_it(i.begin), end_it(i.end) {}

  auto begin() const { return begin_it; }
  auto end() const { return end_it; }

  uint64_t count(Predicate pred) const {
    uint64_t c = 0;
    for (auto&& v : *this) {
      if (pred(v)) {
        c++;
      }
    }

    return c;
  }

  auto select(Predicate pred) const {
    return Enumerable<enumerable::select::iterator<I, T>>(
      enumerable::select::iterator(begin_it, end_it, pred),
      enumerable::select::iterator(end_it, end_it, pred)
    );
  }

  auto find(Predicate pred) const {
    for (auto&& v : *this) {
      if (pred(v)) {
        return std::optional(v);
      }
    }

    return std::optional<T>();
  }

  bool any(Predicate pred) const {
    return find([&pred](auto&& v) { return pred(v); }).has_value();
  }

  bool all(Predicate pred) const {
    return !any([&pred](auto&& v) { return !pred(v); });
  }

  bool contains(const T &value) const {
    return any([&value](auto&& v) { return value == v; });
  }

  template<typename F> auto map(F fn) const {
    typedef decltype(fn(std::declval<T>())) To;

    return Enumerable<enumerable::map::iterator<I, T, To>>(
      enumerable::map::iterator<I, T, To>(begin_it, end_it, fn),
      enumerable::map::iterator<I, T, To>(end_it, end_it, fn)
    );
  }

  template<typename K, typename F> auto foldl(const K &initial, F op) const {
    auto r = initial;
    for (auto && v : *this) {
      r = op(r, v);
    }
    return r;
  }

  template<typename F> auto foldl(F op) const {
    if (begin_it == end_it) {
      return std::optional<T>();
    }

    auto r = *begin_it;
    auto it = begin_it;
    for (++it; it != end_it; ++it) {
      r = op(r, *it);
    }

    return std::make_optional(r);
  }

  auto min() const {
    return foldl([](auto t, auto v) { return t < v ? t : v; });
  }

  template<typename F> auto min_by(F op) const {
    return foldl([&op](auto t, auto v) { return op(t) < op(v) ? t : v; });
  }

  auto max() const {
    return foldl([](auto t, auto v) { return t < v ? v : t; });
  }

  template<typename F> auto max_by(F op) const {
    return foldl([&op](auto t, auto v) { return op(t) < op(v) ? v : t; });
  }

  /*
  each

  template <typename F, typename U> auto map(U (F::*pred)()) const {
    return map([&](auto e){ return (e.*pred)(); });
  }

  template <typename F, typename U> auto map(U (F::*pred)() const) const {
    return map([&](auto e){ return (e.*pred)(); });
  }

  template <typename F, typename U> auto map(U F::*pred) const {
    return map([&](auto e){ return e.*pred; });
  }

  template <typename F, template<typename> class Container=std::vector> auto group_by(F key) const {
    typedef decltype(key(std::declval<T>())) K;
    std::map<K, Container<T>> grouped;

    for (auto&& v : *this) {
      grouped[key(v)].push_back(v);
    }

    return grouped;
  }
  */

private:
  I begin_it;
  I end_it;
};

#endif
