#include "horner_model.hpp"

#include <array>
#include <vector>

#include <glm/vec3.hpp>
#include <gtest/gtest.h>

#include <milt/nurbs/power_basis.hpp>

namespace {

// A cubic with no zero components, so every term moves every coordinate. The values are
// chosen so that partial sums at u = 0, 0.5 and 1 are exact in double.
constexpr std::array cubic{glm::dvec3{1.0, -2.0, 0.5}, glm::dvec3{3.0, 1.0, -1.0},
                           glm::dvec3{-2.0, 4.0, 2.0}, glm::dvec3{0.5, -3.0, 1.5}};

} // namespace

TEST(TermChain, HasOnePointPerCoefficient) {
    EXPECT_EQ(term_chain(cubic, 0.5).size(), cubic.size());
}

TEST(TermChain, CollapsesToA0AtZero) {
    for (const glm::dvec3& p : term_chain(cubic, 0.0)) {
        EXPECT_EQ(p, cubic[0]);
    }
}

TEST(TermChain, PartialSumsAtHalf) {
    // p0 = a0, p1 = p0 + a1 / 2, p2 = p1 + a2 / 4, p3 = p2 + a3 / 8.
    const std::vector<glm::dvec3> chain = term_chain(cubic, 0.5);
    EXPECT_EQ(chain[0], glm::dvec3(1.0, -2.0, 0.5));
    EXPECT_EQ(chain[1], glm::dvec3(2.5, -1.5, 0.0));
    EXPECT_EQ(chain[2], glm::dvec3(2.0, -0.5, 0.5));
    EXPECT_EQ(chain[3], glm::dvec3(2.0625, -0.875, 0.6875));
}

TEST(TermChain, StepsAreCoefficientsAtOne) {
    const std::vector<glm::dvec3> chain = term_chain(cubic, 1.0);
    EXPECT_EQ(chain[1] - chain[0], cubic[1]);
    EXPECT_EQ(chain[2] - chain[1], cubic[2]);
    EXPECT_EQ(chain[3] - chain[2], cubic[3]);
}

TEST(TermChain, TipIsTheCurvePoint) {
    // The view draws a marker at horner1's result, so the chain must end exactly on it.
    for (const double u : {0.1, 0.3, 0.7, 0.9}) {
        SCOPED_TRACE(u);
        EXPECT_EQ(term_chain(cubic, u).back(), milt::nurbs::horner1(cubic, u));
    }
}

TEST(TermChain, DegreeZeroIsASinglePoint) {
    const std::array constant{glm::dvec3{4.0, 5.0, 6.0}};
    const std::vector<glm::dvec3> chain = term_chain(constant, 0.7);
    ASSERT_EQ(chain.size(), 1U);
    EXPECT_EQ(chain[0], constant[0]);
}
