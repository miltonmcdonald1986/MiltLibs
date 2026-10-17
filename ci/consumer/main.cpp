#include <milt/nurbs/nurbs.hpp>

#include <array>

// 1 + 2u + 3u^2 at u = 2 is 17. Checked at compile time, so building is the test.
constexpr std::array coefficients{1.0, 2.0, 3.0};
static_assert(milt::nurbs::horner1(coefficients, 2.0) == 17.0);

int main() {}
