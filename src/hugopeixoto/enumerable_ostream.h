#ifndef HUGOPEIXOTO_ENUMERABLE_OSTREAM_H_
#define HUGOPEIXOTO_ENUMERABLE_OSTREAM_H_

#include <ostream>

template <typename T, template <typename> class C>
std::ostream &operator<<(std::ostream &out,
                         const Enumerable<T, C> &enumerable) {
  bool first = true;

  out << "(";
  enumerable.each([&out, &first](const auto &e) {
    if (!first)
      out << ", ";
    out << e;
    first = false;
  });

  return out << ")";
}

template <typename F, typename S>
std::ostream &operator<<(std::ostream &out, const std::pair<F, S> &p) {
  return out << "(" << p.first << ", " << p.second << ")";
}

#endif
