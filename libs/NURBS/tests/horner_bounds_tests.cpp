#include <cmath>
#include <cstddef>

#include <glm/vec3.hpp>
#include <glm/vector_relational.hpp>
#include <gtest/gtest.h>

#include "horner_model.hpp"
#include "horner_presets.hpp"

namespace {

constexpr std::size_t samples = 65;

bool contains(const Bounds& bounds, glm::dvec3 p) {
    return glm::all(glm::lessThanEqual(bounds.min, p)) &&
           glm::all(glm::lessThanEqual(p, bounds.max));
}

} // namespace

TEST(FitBounds, HoldsEveryChainPointAtTheSamples) {
    // 1/4 steps are among the 65 samples, which are spaced 1/64 apart.
    for (const CurvePreset& preset : curve_presets) {
        SCOPED_TRACE(preset.name);
        const Bounds bounds = fit_bounds(preset.coefficients, samples);
        for (const double u : {0.0, 0.25, 0.5, 0.75, 1.0}) {
            SCOPED_TRACE(u);
            for (const glm::dvec3& p : term_chain(preset.coefficients, u)) {
                EXPECT_TRUE(contains(bounds, p));
            }
        }
    }
}

TEST(FitBounds, LineIsItsSegmentsBox) {
    // A line's chain is a0 and the curve point, so the box is just the segment's.
    const Bounds bounds = fit_bounds(curve_presets[0].coefficients, samples);
    EXPECT_EQ(bounds.min, glm::dvec3(-1.0, 0.0, 0.0));
    EXPECT_EQ(bounds.max, glm::dvec3(1.0, 1.0, 0.0));
}

TEST(FitBounds, IncludesArrowsThatLeaveTheCurve) {
    // The arc never rises above y = 0.5, but at u = 1 the chain's first arrow, a1 = (2, 2, 0),
    // ends at (1, 2, 0) before a2 brings it back down.
    const Bounds bounds = fit_bounds(curve_presets[1].coefficients, samples);
    EXPECT_EQ(bounds.min, glm::dvec3(-1.0, 0.0, 0.0));
    EXPECT_EQ(bounds.max, glm::dvec3(1.0, 2.0, 0.0));
}

TEST(FitBounds, TwistedCubicFillsTheCube) {
    const Bounds bounds = fit_bounds(curve_presets[2].coefficients, samples);
    EXPECT_EQ(bounds.min, glm::dvec3(-1.0, -1.0, -1.0));
    EXPECT_EQ(bounds.max, glm::dvec3(1.0, 1.0, 1.0));
    EXPECT_EQ(bounds.center(), glm::dvec3(0.0, 0.0, 0.0));
    EXPECT_DOUBLE_EQ(bounds.radius(), std::sqrt(3.0));
}

TEST(FitBounds, TwoSamplesAreTheEnds) {
    // With only u = 0 and u = 1, the box holds the chains at the two ends and nothing else.
    const Bounds bounds = fit_bounds(curve_presets[1].coefficients, 2);
    EXPECT_EQ(bounds.min, glm::dvec3(-1.0, 0.0, 0.0));
    EXPECT_EQ(bounds.max, glm::dvec3(1.0, 2.0, 0.0));
}
