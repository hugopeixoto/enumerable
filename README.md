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

class Range : public Enumerable<int, Container> {
public:
  typedef int T;

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
  auto enumerable = Range(4, 14);

  auto squares = enumerable.map([](auto i) { return i * i; });

  auto minimum = enumerable.min();
  auto maximum = enumerable.max();

  auto sum = enumerable.foldl(0, [](auto prev, auto v) { return prev + v; });

  auto grouped_by_parity = enumerable.group_by([](auto i) { return i & 1; });

  std::cout << "squared: " << squares << std::endl;

  std::cout << "min: " << minimum << std::endl;
  std::cout << "max: " << maximum << std::endl;

  std::cout << "sum: " << sum << std::endl;

  std::cout << "grouped: " << grouped_by_parity << std::endl;

  return 0;
}
```

Output:

```
squared: (16, 25, 36, 49, 64, 81, 100, 121, 144, 169)
min: 4
max: 13
sum: 85
grouped: ((0, (4, 6, 8, 10, 12)), (1, (5, 7, 9, 11, 13)))
```
