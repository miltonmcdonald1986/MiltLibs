/// @file
/// @brief Algorithms for curves in power basis form (The NURBS Book, chapter 1).

#pragma once

#include <cassert>
#include <ranges>

#include <milt/nurbs/point.hpp>

namespace milt::nurbs {

/// @brief Evaluates a power basis curve at a parameter value using Horner's method.
///
/// Computes \f$ C(u_0) = \sum_{i=0}^{n} a_i u_0^i \f$ in \f$ O(n) \f$ operations.
/// This is algorithm A1.1 (Horner1) from *The NURBS Book*.
///
/// @tparam Real         The scalar type of the parameter.
/// @tparam Coefficients A range of points; see PointRange.
///
/// @param coefficients The coefficients \f$ a_0, \dots, a_n \f$, lowest power first.
/// @param u0           The parameter value at which to evaluate the curve.
///
/// @return The point \f$ C(u_0) \f$.
///
/// @pre `coefficients` is not empty (the curve has degree \f$ n \ge 0 \f$). Checked with
///      `assert`.
template <class Real, PointRange<Real> Coefficients>
[[nodiscard]] constexpr point_t<Coefficients> horner1(const Coefficients& coefficients, Real u0) {
    assert(!std::ranges::empty(coefficients));

    point_t<Coefficients> c = *std::ranges::rbegin(coefficients);
    for (const auto& a : coefficients | std::views::reverse | std::views::drop(1)) {
        c = c * u0 + a;
    }
    return c;
}

} // namespace milt::nurbs
