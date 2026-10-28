#include "horner_presets.hpp"

#include <algorithm>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <gtest/gtest.h>

#include <milt/nurbs/power_basis.hpp>

namespace {

using milt::nurbs::horner1;

const CurvePreset& line = curve_presets[0];
const CurvePreset& parabolic_arc = curve_presets[1];
const CurvePreset& twisted_cubic = curve_presets[2];

} // namespace

TEST(CurvePresets, DegreesAreOneTwoThree) {
    // Degree n has n + 1 coefficients.
    ASSERT_EQ(curve_presets.size(), 3U);
    EXPECT_EQ(line.coefficients.size(), 2U);
    EXPECT_EQ(parabolic_arc.coefficients.size(), 3U);
    EXPECT_EQ(twisted_cubic.coefficients.size(), 4U);
}

TEST(CurvePresets, Is3dMatchesCoefficients) {
    // A preset is 2D exactly when every coefficient, and so the whole curve, has z = 0.
    for (const CurvePreset& preset : curve_presets) {
        SCOPED_TRACE(preset.name);
        const bool leaves_plane = std::ranges::any_of(
            preset.coefficients, [](const glm::dvec3& a) { return a.z != 0.0; });
        EXPECT_EQ(preset.is_3d, leaves_plane);
    }
}

TEST(CurvePresets, LineEndpoints) {
    EXPECT_EQ(horner1(line.coefficients, 0.0), glm::dvec3(-1.0, 0.0, 0.0));
    EXPECT_EQ(horner1(line.coefficients, 1.0), glm::dvec3(1.0, 1.0, 0.0));
}

TEST(CurvePresets, ParabolicArcEndpointsAndApex) {
    EXPECT_EQ(horner1(parabolic_arc.coefficients, 0.0), glm::dvec3(-1.0, 0.0, 0.0));
    EXPECT_EQ(horner1(parabolic_arc.coefficients, 1.0), glm::dvec3(1.0, 0.0, 0.0));
    EXPECT_EQ(horner1(parabolic_arc.coefficients, 0.5), glm::dvec3(0.0, 0.5, 0.0));
}

TEST(CurvePresets, TwistedCubicIsNotPlanar) {
    // The curve is planar exactly when a1, a2 and a3 are linearly dependent, which is when
    // their triple product is zero. Here it is det(diag(2, 2, 2)) = 8.
    const auto a = twisted_cubic.coefficients;
    EXPECT_EQ(glm::dot(a[1], glm::cross(a[2], a[3])), 8.0);
}

TEST(CurvePresets, TwistedCubicPoints) {
    for (const double u : {0.0, 0.5, 1.0}) {
        SCOPED_TRACE(u);
        EXPECT_EQ(horner1(twisted_cubic.coefficients, u),
                  glm::dvec3(-1.0 + 2.0 * u, -1.0 + 2.0 * u * u, -1.0 + 2.0 * u * u * u));
    }
}
