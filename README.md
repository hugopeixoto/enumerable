enumerable
==========

Enumerable class similar to the Ruby one. Just inherit from it and implement
the `each` method.  You must also specify a container class which must respond
to `push_back(T)`. This is used to store results from `map`, for example. It
is generally a good idea that this container classe inherits from Enumerable as well.

In Ruby, the equivalent to the Container class is `Array`.

Example usage:

```c++
#include "hugopeixoto/enumerable.h"
#include "hugopeixoto/enumerable_ostream.h"

#include <iostream>
#include <vector>

template <typename T>
class Container : public std::vector<T>, public Enumerable<T, Container> {
public:
  virtual void each(std::function<void(const T &)> pred) const override {
    for (const T &e : *this) {
      pred(e);
    }
  }
};

template<typename T>
class Range : public Enumerable<T, Container> {
public:
  Range(T lower, T upper) : lower(lower), upper(upper) {}

  virtual void each(std::function<void(const T &)> callback) const {
    for (T i = lower; i < upper; i++) {
      callback(i);
    }
  }

protected:
  T lower;
  T upper;
};

int main() {
  auto is_even = [](auto i) { return (i % 2) == 0; };
  auto square  = [](auto i) { return i * i; };
  auto add     = [](auto i, auto j) { return i + j; };

  auto enumerable = Range<int>(4, 14);

  std::cout << "squared: " << enumerable.map(square) << std::endl;

  std::cout << "min    : " << enumerable.min().orDefault(-1) << std::endl;
  std::cout << "max    : " << enumerable.max().orDefault(-1) << std::endl;

  std::cout << "sum    : " << enumerable.foldl(0, add) << std::endl;

  std::cout << "count  : " << enumerable.count(is_even) << std::endl;
  std::cout << "grouped: " << enumerable.group_by(is_even) << std::endl;

  return 0;
}
```

Output:

```
squared: (16, 25, 36, 49, 64, 81, 100, 121, 144, 169)
min    : 4
max    : 13
sum    : 85
count  : 5
grouped: ((0, (5, 7, 9, 11, 13)), (1, (4, 6, 8, 10, 12)))
```
