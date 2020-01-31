#include "hugopeixoto/enumerable.h"
#include "hugopeixoto/enumerable_ostream.h"

#include <iostream>
#include <vector>

template<typename T>
class Range : public Enumerable<T> {
public:
  Range(const T& f, const T& s) : first(f), second(s) {}

  T first, second;

  virtual void each(std::function<void(const T &)> callback) const {
    for (T i = this->first; i < this->second; i++) {
      callback(i);
    }
  }
};

int main() {
  auto is_even = [](auto i) { return (i % 2) == 0; };
  auto square  = [](auto i) { return i * i; };
  auto add     = [](auto i, auto j) { return i + j; };

  auto enumerable = Range<int>(4, 14);

  std::cout << "source : " << enumerable << std::endl;
  std::cout << "squared: " << enumerable.map(square) << std::endl;

  std::cout << "min    : " << enumerable.min().value_or(-1) << std::endl;
  std::cout << "max    : " << enumerable.max().value_or(-1) << std::endl;

  std::cout << "sum    : " << enumerable.foldl(0, add) << std::endl;

  std::cout << "select : " << enumerable.select(is_even) << std::endl;
  std::cout << "count  : " << enumerable.count(is_even) << std::endl;
  std::cout << "grouped: " << enumerable.group_by(is_even) << std::endl;

  return 0;
}
