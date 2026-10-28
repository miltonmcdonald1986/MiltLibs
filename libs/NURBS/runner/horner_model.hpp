/// @file
/// @brief The power basis term chain drawn by the NURBS runner, and the box that frames it,
/// without any drawing code.

#pragma once

#include <cassert>
#include <cstddef>
#include <span>
#include <vector>

#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <milt/nurbs/power_basis.hpp>

/// @brief Returns the points where each term of a power basis curve ends when the terms are
/// laid tip to tail.
///
/// For \f$ C(u) = \sum_{k=0}^{n} a_k u^k \f$, point \f$ p_k \f$ is the partial sum
/// \f$ \sum_{i=0}^{k} a_i u^i \f$, so the chain starts at \f$ a_0 \f$ and each step adds the
/// term \f$ u^k a_k \f$. Drawing an arrow from each point to the next shows how the evaluation
/// reaches \f$ C(u) \f$, which is the last point.
///
/// Each partial sum is evaluated with `milt::nurbs::horner1` on the first \f$ k + 1 \f$
/// coefficients, so the last point is exactly what `horner1` returns for the whole curve. That
/// costs \f$ O(n^2) \f$, which is nothing at the degrees the runner shows.
///
/// @param coefficients The coefficients \f$ a_0, \dots, a_n \f$, lowest power first.
/// @param u            The parameter value.
///
/// @return The \f$ n + 1 \f$ points \f$ p_0, \dots, p_n \f$.
///
/// @pre `coefficients` is not empty. Checked with `assert`.
[[nodiscard]] inline std::vector<glm::dvec3> term_chain(std::span<const glm::dvec3> coefficients,
                                                        double u) {
    assert(!coefficients.empty());

    std::vector<glm::dvec3> chain;
    chain.reserve(coefficients.size());
    for (std::size_t k = 0; k < coefficients.size(); ++k) {
        chain.push_back(milt::nurbs::horner1(coefficients.first(k + 1), u));
    }
    return chain;
}

/// @brief An axis-aligned box.
struct Bounds {
    glm::dvec3 min; ///< The corner with the smallest coordinates.
    glm::dvec3 max; ///< The corner with the largest coordinates.

    /// @brief Returns the middle of the box.
    [[nodiscard]] constexpr glm::dvec3 center() const { return (min + max) * 0.5; }

    /// @brief Returns half the box's diagonal: the radius of the smallest sphere around
    /// center() that holds the box, whatever direction it's viewed from.
    [[nodiscard]] double radius() const { return glm::length(max - min) * 0.5; }
};

/// @brief Returns a box around everything the runner draws for a curve as u moves over
/// [0, 1]: the curve itself and every point of its term chain.
///
/// The chain can swing well outside the curve, so fitting the view to this box keeps the
/// arrows on screen for every u. The box is computed from evenly spaced samples of u,
/// including 0 and 1, so it can miss a sliver between samples; leave a margin around it.
///
/// @param coefficients The coefficients \f$ a_0, \dots, a_n \f$, lowest power first.
/// @param samples      The number of u values to sample.
///
/// @return The smallest box holding the term chain at every sampled u.
///
/// @pre `coefficients` is not empty and `samples >= 2`. Checked with `assert`.
[[nodiscard]] inline Bounds fit_bounds(std::span<const glm::dvec3> coefficients,
                                       std::size_t samples) {
    assert(!coefficients.empty());
    assert(samples >= 2);

    Bounds bounds{.min = coefficients.front(), .max = coefficients.front()};
    for (std::size_t i = 0; i < samples; ++i) {
        const double u = static_cast<double>(i) / static_cast<double>(samples - 1);
        for (const glm::dvec3& p : term_chain(coefficients, u)) {
            bounds.min = glm::min(bounds.min, p);
            bounds.max = glm::max(bounds.max, p);
        }
    }
    return bounds;
}
