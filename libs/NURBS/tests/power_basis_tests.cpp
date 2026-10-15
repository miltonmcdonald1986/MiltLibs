#include <milt/nurbs/power_basis.hpp>

#include <array>
#include <concepts>
#include <forward_list>
#include <list>
#include <span>
#include <vector>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gtest/gtest.h>

namespace {

using milt::nurbs::horner1;

struct NoOps {};

static_assert(milt::nurbs::Point<double, double>);
static_assert(milt::nurbs::Point<glm::dvec2, double>);
static_assert(milt::nurbs::Point<glm::dvec3, double>);
static_assert(!milt::nurbs::Point<NoOps, double>);
static_assert(!milt::nurbs::PointRange<std::forward_list<double>, double>);
static_assert(!milt::nurbs::PointRange<std::list<double>, double>);

// horner1 returns the range's point type.
static_assert(std::same_as<milt::nurbs::point_t<std::vector<glm::dvec2>>, glm::dvec2>);
static_assert(std::same_as<decltype(horner1(std::vector<glm::dvec2>{}, 0.5)), glm::dvec2>);

// 1 + 2u + 3u^2 at u = 2 is 17, exact in double.
constexpr std::array scalar_coefficients{1.0, 2.0, 3.0};
static_assert(horner1(scalar_coefficients, 2.0) == 17.0);

} // namespace

TEST(Horner1, ScalarPolynomial) {
    EXPECT_DOUBLE_EQ(horner1(scalar_coefficients, 0.0), 1.0);
    EXPECT_DOUBLE_EQ(horner1(scalar_coefficients, 1.0), 6.0);
    EXPECT_DOUBLE_EQ(horner1(scalar_coefficients, 2.0), 17.0);
    EXPECT_DOUBLE_EQ(horner1(scalar_coefficients, -1.0), 2.0);
}

TEST(Horner1, DegreeZeroIsConstant) {
    const std::array coefficients{4.5};
    EXPECT_DOUBLE_EQ(horner1(coefficients, 0.0), 4.5);
    EXPECT_DOUBLE_EQ(horner1(coefficients, 0.7), 4.5);
    EXPECT_DOUBLE_EQ(horner1(coefficients, 100.0), 4.5);
}

TEST(Horner1, DegreeOneIsLineSegment) {
    // Segment from P0 = (1, 2) to P1 = (4, 6): a0 = P0, a1 = P1 - P0 = (3, 4).
    const std::array coefficients{glm::dvec2{1.0, 2.0}, glm::dvec2{3.0, 4.0}};

    const glm::dvec2 start = horner1(coefficients, 0.0);
    EXPECT_DOUBLE_EQ(start.x, 1.0);
    EXPECT_DOUBLE_EQ(start.y, 2.0);

    const glm::dvec2 end = horner1(coefficients, 1.0);
    EXPECT_DOUBLE_EQ(end.x, 4.0);
    EXPECT_DOUBLE_EQ(end.y, 6.0);

    const glm::dvec2 mid = horner1(coefficients, 0.5);
    EXPECT_DOUBLE_EQ(mid.x, 2.5);
    EXPECT_DOUBLE_EQ(mid.y, 4.0);
}

TEST(Horner1, DegreeTwoIsParabolicArc) {
    // y = x^2 for x in [-1, 1], with x = -1 + 2u:
    // C(u) = (-1, 1) + (2, -4) u + (0, 4) u^2.
    const std::array coefficients{glm::dvec2{-1.0, 1.0}, glm::dvec2{2.0, -4.0},
                                  glm::dvec2{0.0, 4.0}};

    for (const double u : {0.0, 0.25, 0.5, 0.75, 1.0}) {
        const glm::dvec2 c = horner1(coefficients, u);
        EXPECT_DOUBLE_EQ(c.x, -1.0 + 2.0 * u);
        EXPECT_DOUBLE_EQ(c.y, c.x * c.x);
    }

    const glm::dvec2 vertex = horner1(coefficients, 0.5);
    EXPECT_DOUBLE_EQ(vertex.x, 0.0);
    EXPECT_DOUBLE_EQ(vertex.y, 0.0);
}

TEST(Horner1, DegreeThreeIsTwistedCubic) {
    // The twisted cubic C(u) = (u, u^2, u^3), which lies in no single plane.
    const std::array coefficients{glm::dvec3{0.0, 0.0, 0.0}, glm::dvec3{1.0, 0.0, 0.0},
                                  glm::dvec3{0.0, 1.0, 0.0}, glm::dvec3{0.0, 0.0, 1.0}};

    for (const double u : {0.0, 0.5, 1.0, 2.0, 3.0}) {
        const glm::dvec3 c = horner1(coefficients, u);
        EXPECT_DOUBLE_EQ(c.x, u);
        EXPECT_DOUBLE_EQ(c.y, u * u);
        EXPECT_DOUBLE_EQ(c.z, u * u * u);
    }

    // Four points are coplanar iff the triple product of their differences is zero.
    // For u = 0, 1, 2, 3 it is the Vandermonde determinant 1 * 2 * 3 * (2-1)(3-1)(3-2) = 12.
    const glm::dvec3 p0 = horner1(coefficients, 0.0);
    const glm::dvec3 p1 = horner1(coefficients, 1.0);
    const glm::dvec3 p2 = horner1(coefficients, 2.0);
    const glm::dvec3 p3 = horner1(coefficients, 3.0);
    EXPECT_DOUBLE_EQ(glm::dot(p1 - p0, glm::cross(p2 - p0, p3 - p0)), 12.0);
}

TEST(Horner1, UserPointType) {
    // C(u) = (1, 0) + (0, 2) u + (1, 1) u^2
    const std::array coefficients{glm::dvec2{1.0, 0.0}, glm::dvec2{0.0, 2.0}, glm::dvec2{1.0, 1.0}};

    const glm::dvec2 c0 = horner1(coefficients, 0.0);
    EXPECT_DOUBLE_EQ(c0.x, 1.0);
    EXPECT_DOUBLE_EQ(c0.y, 0.0);

    const glm::dvec2 c2 = horner1(coefficients, 2.0);
    EXPECT_DOUBLE_EQ(c2.x, 5.0);
    EXPECT_DOUBLE_EQ(c2.y, 8.0);

    const glm::dvec2 half = horner1(coefficients, 0.5);
    EXPECT_DOUBLE_EQ(half.x, 1.25);
    EXPECT_DOUBLE_EQ(half.y, 1.25);
}

TEST(Horner1, AcceptsCommonContainers) {
    const std::vector<double> vec{1.0, 2.0, 3.0};
    const std::span<const double> span{vec};
    const double c_array[]{1.0, 2.0, 3.0};

    EXPECT_DOUBLE_EQ(horner1(vec, 2.0), 17.0);
    EXPECT_DOUBLE_EQ(horner1(span, 2.0), 17.0);
    EXPECT_DOUBLE_EQ(horner1(c_array, 2.0), 17.0);
}

TEST(Horner1, FloatScalar) {
    const std::array coefficients{1.0f, 2.0f, 3.0f};
    const float c = horner1(coefficients, 2.0f);
    EXPECT_FLOAT_EQ(c, 17.0f);
}
