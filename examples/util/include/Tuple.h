#include <iostream>

template <typename T, int N>
class Tuple {

  T e[N];

public:
  
  Tuple(T s = 0) { for (int i = 0; i < N; i++) e[i] = s; }

  Tuple(const T* v) { for (int i = 0; i < N; i++) e[i] = v[i]; }

  Tuple(const Tuple& t) { for (int i = 0; i < N; i++) e[i] = t.e[i]; }

  Tuple& operator=(const Tuple& t) {
    for (int i = 0; i < N; i++) e[i] = t.e[i];
    return *this;
  }

  Tuple &operator=(T s) {
    for (int i = 0; i < N; i++) e[i] = s;
    return *this;
  }

  const T& operator[](int i) const {
    return e[i];
  }

  friend const Tuple operator+(const Tuple& lhs, const Tuple& rhs) {
    Tuple t;
    for (int i = 0; i < N; i++) { t.e[i] = lhs.e[i] + rhs.e[i]; }
    return t;
  }

  friend const Tuple operator+(const Tuple& lhs, const T rhs) {
    Tuple t;
    for (int i = 0; i < N; i++) { t.e[i] = lhs.e[i] + rhs; }
    return t;
  }

  friend std::ostream& operator<<(std::ostream& os, const Tuple& t) {
    for (int i = 0; i < N; i++) {
      if (i < N - 1) os << t.e[i] << " ";
      else           os << t.e[i];
    }
    return os;
  }

  friend std::istream& operator>>(std::istream& is, Tuple& t) {
    for (int i = 0; i < N; i++) is >> t.e[i];
    return is;
  }

};
