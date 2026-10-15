/// @file
/// @brief Concepts describing the point types the NURBS algorithms operate on.

#pragma once

#include <concepts>
#include <ranges>

namespace milt::nurbs {

/// @brief A user-supplied point (or vector) type that can be scaled by @p Real and added.
///
/// Every curve algorithm in this library is written against these operations, so any type
/// providing them can be used, including plain scalars such as `double`.
///
/// @tparam P    The point type.
/// @tparam Real The scalar type points are multiplied by, on the right (`p * r`).
template <class P, class Real>
concept Point = std::copyable<P> && requires(const P& p, const Real& r) {
    { p * r } -> std::convertible_to<P>;
    { p + p } -> std::convertible_to<P>;
};

/// @brief The point type held by a range of points.
/// @tparam R A range, e.g. `std::vector<Vec3>`.
template <class R>
using point_t = std::ranges::range_value_t<R>;

/// @brief A random-access sequence of points (coefficients, control points) usable with
/// scalar @p Real.
///
/// Random access is required because algorithms such as B-spline evaluation index points
/// directly; every algorithm in the library accepts this one concept.
///
/// @tparam R    The range type, e.g. `std::vector`, `std::array`, `std::span` or a C array.
/// @tparam Real The scalar type the points are multiplied by.
template <class R, class Real>
concept PointRange = std::ranges::random_access_range<R> && Point<point_t<R>, Real>;

} // namespace milt::nurbs
