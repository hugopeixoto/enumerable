#ifndef HUGOPEIXOTO_ENUMERABLE_OSTREAM_H_
#define HUGOPEIXOTO_ENUMERABLE_OSTREAM_H_

#include <ostream>

template <typename T>
std::ostream &operator<<(std::ostream &out, const Enumerable<T>& enumerable) {
  bool first = true;

  out << "(";
  for (auto &&v : enumerable) {
    if (!first) out << ", ";
    out << v;
    first = false;
  }

  return out << ")";
}

namespace std {
  template <typename F, typename S>
    ostream &operator<<(ostream &out, const pair<F, S> &p) {
      return out << "(" << p.first << ", " << p.second << ")";
    }
}

#endif
