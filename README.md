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

  enumerable.map<int>([](auto i) { return i * i; })
      .each([](auto i) { std::cout << "squared: " << i << std::endl; });

  std::cout << "min: " << enumerable.min() << std::endl;
  std::cout << "max: " << enumerable.max() << std::endl;

  enumerable.group_by<int>([](auto i) { return i & 1; }).each([](auto group) {
    std::cout << "group " << group.first << ":";
    group.second.each([](auto v) { std::cout << " " << v; });
    std::cout << std::endl;
  });

  std::cout << "foldl(0, +): "
            << enumerable.foldl<int>(0, [](const int &prev, const int &v) {
                 return prev + v;
               }) << std::endl;

  return 0;
}
```
