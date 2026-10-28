/// @file
/// @brief The power basis term chain drawn by the NURBS runner, without any drawing code.

#pragma once

#include <cassert>
#include <cstddef>
#include <span>
#include <vector>

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
