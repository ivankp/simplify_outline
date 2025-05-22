#include <iostream>
#include <cmath>

#define STR1(x) #x
#define STR(x) STR1(x)

#define TEST(var) std::cout << \
  "\033[33m" STR(__LINE__) ": " \
  "\033[36m" #var ":\033[0m " << (var) << std::endl;

using std::cout, std::endl;

template <typename... T>
constexpr auto sq(const T&... x) noexcept {
  return ( (x*x) + ... );
}

struct Point { double x, y; };

constexpr Point operator+(const Point& a, const Point& b) noexcept {
  return { a.x + b.x, a.y + b.y };
}
constexpr Point operator-(const Point& a, const Point& b) noexcept {
  return { a.x - b.x, a.y - b.y };
}
constexpr double operator*(const Point& a, const Point& b) noexcept {
  return a.x * b.x + a.y * b.y;
}
constexpr double operator^(const Point& a, const Point& b) noexcept {
  return a.x * b.y - a.y * b.x;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  // must be CCW
  const Point outline[] {
    { 0, 0 },
    { 2, 0 },
    { 2, 2 },
    { 1, 2.09 },
    { 0, 2 },
    { 0, 1.5 },
    { 0, 1.0 },
  };
  const size_t nPoints = std::size(outline);

  // TEST(nPoints);

  bool* const mask = new bool[nPoints];

  // Is there a way to loop to help the processor with branch prediction?
  for (size_t a = 0; a < nPoints; ++a) {
    size_t b = a + 1, c = a + 2;
    if (c >= nPoints) {
      c -= nPoints;
      if (b >= nPoints) {
        b -= nPoints;
      }
    }
    // TEST(a)
    // TEST(b)
    // TEST(c)

    Point v[] {
      outline[a] - outline[b],
      outline[c] - outline[b]
    };

    const double mag = std::sqrt(sq(v[0]) * sq(v[1]));

    const double cross = v[0] * v[1];
    const double cos = cross / mag;
    const double wedge = v[0] ^ v[1];
    const double sin = wedge / mag;

    TEST(cos)
    TEST(sin)

    mask[b] = cos < 0 && std::abs(sin) < 0.2;
  }

  for (size_t i = 0; i < nPoints; ++i) {
    TEST(mask[i])
  }
}
