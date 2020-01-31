#include "hugopeixoto/enumerable.h"
#include "hugopeixoto/enumerable_ostream.h"

#include <iostream>
#include <vector>

template<typename T>
class Range {
public:
  class iterator {
  public:
    iterator(T v) : v(v) {}
    iterator(const iterator& it) : v(it.v) {}
    T operator*() const { return v; }
    iterator& operator++() { ++this->v; return *this; }
    iterator& operator++(int) { iterator ret = *this; ++(*this); return ret; }
    bool operator==(const iterator& other) const { return v == other.v; }
    bool operator!=(const iterator& other) const { return !(*this == other); }

    // using difference_type = ???
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = std::forward_iterator_tag;
  private:
    T v;
  };

  Range(const T& a, const T& b) : first(a), second(b) {}
  const T first, second;

  iterator begin() { return iterator(first); };
  iterator end() { return iterator(second); };
};

int main() {
  auto is_even = [](auto i) { return (i % 2) == 0; };
  auto square  = [](auto i) { return i * i; };
  auto neg     = [](auto i) { return -i; };
  auto add     = [](auto i, auto j) { return i + j; };

  auto range = Range<int>(4, 14);
  auto enumerable = Enumerable(range.begin(), range.end());

  std::cout << "source  : " << enumerable << std::endl;
  std::cout << "count   : " << enumerable.count(is_even) << std::endl;
  std::cout << "select  : " << enumerable.select(is_even) << std::endl;
  std::cout << "map     : " << enumerable.map(square) << std::endl;
  std::cout << "foldl   : " << enumerable.foldl(0, add) << std::endl;
  std::cout << "foldl   : " << enumerable.foldl(add).value_or(-1) << std::endl;
  std::cout << "min     : " << enumerable.min().value_or(-1) << std::endl;
  std::cout << "min_by  : " << enumerable.min_by(neg).value_or(-1) << std::endl;
  std::cout << "max     : " << enumerable.max().value_or(-1) << std::endl;
  std::cout << "max_by  : " << enumerable.max_by(neg).value_or(-1) << std::endl;
  std::cout << "find    : " << enumerable.find(is_even).value_or(-1) << std::endl;
  std::cout << "any     : " << enumerable.any(is_even) << std::endl;
  std::cout << "contains: " << enumerable.contains(9) << std::endl;

  return 0;
}
