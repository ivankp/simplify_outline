#include <iostream>
#include <cmath>

#define STR1(x) #x
#define STR(x) STR1(x)

#define TEST(var) std::cout << \
  "\033[33m" STR(__LINE__) ": " \
  "\033[36m" #var ":\033[0m " << (var) << std::endl;

using std::cout, std::endl;

template <typename Outline>
void RemoveColinearPoints(
    Outline& outline,
    double angleTolerance = 1 // degrees
) {
  const size_t nPoints = outline.size();
  if (nPoints < 4)
    return;

  // convert angle tolerance to cosine tolerance
  static const double pi = 3.14159265358979323846;
  angleTolerance = std::cos(pi - (pi/180.) * angleTolerance);
  TEST(angleTolerance)

  size_t
    i0 = 0, // point to which the current point is moved
    i1 = nPoints - 1, // preceeding point
    i2 = 0, // point currently being inspected
    i3 = 1; // following point

  while (i2 < nPoints) {
    double
      x1 = outline[i1][0], y1 = outline[i1][1],
      x2 = outline[i2][0], y2 = outline[i2][1],
      x3 = outline[i3][0], y3 = outline[i3][1];

    cout << i2 << ": " << x2 << ", " << y2 << endl;

      // make point 2 the origin
      x1 -= x2; y1 -= y2;
      x3 -= x2; y3 -= y2;

      double cos = x1*x3 + y1*y3; // dot product of vectors 1 and 3
      TEST(cos)
      if (cos >= 0)
        goto keep;

      cos /= std::sqrt((x1*x1 + y1*y1) * (x3*x3 + y3*y3));
      TEST(cos)

      if (cos < angleTolerance)
        goto remove;

keep:
      if (i0 < i2) {
        outline[i0] = std::move(outline[i2]);
      }
      ++i0;
      i1 = i2;
remove:
      ++i2;
      if (++i3 == nPoints) {
        i3 = 0;
      }
  }

  outline.resize(i0);
}

#include <vector>
#include <array>

int main() {
  std::ios_base::sync_with_stdio(false);

  // must be CCW
  std::vector<std::array<double, 2>> outline {
    { 0, 0 },
    { 2, 0 },
    { 2.01, 1 },
    { 2, 2 },
    { 1, 2.001 },
    { 0, 2 },
    { 0, 1.5 },
    { 0, 1.0 },
  };

  RemoveColinearPoints(outline, 1);

  for (const auto& p : outline) {
    cout << p[0] << "  " << p[1] << '\n';
  }
}
