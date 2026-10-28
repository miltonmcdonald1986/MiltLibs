/// @file
/// @brief The curated curves the NURBS runner can show.

#pragma once

#include <array>
#include <span>
#include <string_view>

#include <glm/vec3.hpp>

/// @brief A read-only power basis curve shown by the runner, with text explaining it.
///
/// Coefficients are in math coordinates, with z up.
struct CurvePreset {
    std::string_view name;                    ///< Short name for the preset picker.
    std::string_view caption;                 ///< What to watch while dragging the u slider.
    std::span<const glm::dvec3> coefficients; ///< \f$ a_0, \dots, a_n \f$, lowest power first.
    bool is_3d; ///< Whether the curve leaves the z = 0 plane, so it needs an oblique view.
};

/// @brief Coefficient storage for the presets in curve_presets.
namespace preset_coefficients {

/// @brief The segment from (-1, 0, 0) to (1, 1, 0).
inline constexpr std::array line{glm::dvec3{-1.0, 0.0, 0.0}, glm::dvec3{2.0, 1.0, 0.0}};

/// @brief An arc from (-1, 0, 0) to (1, 0, 0) with its apex (0, 0.5, 0) at u = 1/2.
inline constexpr std::array parabolic_arc{glm::dvec3{-1.0, 0.0, 0.0}, glm::dvec3{2.0, 2.0, 0.0},
                                          glm::dvec3{0.0, -2.0, 0.0}};

/// @brief The twisted cubic \f$ (-1 + 2u, -1 + 2u^2, -1 + 2u^3) \f$, which lies in no plane.
inline constexpr std::array twisted_cubic{glm::dvec3{-1.0, -1.0, -1.0}, glm::dvec3{2.0, 0.0, 0.0},
                                          glm::dvec3{0.0, 2.0, 0.0}, glm::dvec3{0.0, 0.0, 2.0}};

} // namespace preset_coefficients

/// @brief The presets, in the order the picker shows them: one each of degree 1, 2 and 3.
inline constexpr std::array curve_presets{
    CurvePreset{
        .name = "Line",
        .caption = "One arrow, u*a1, grows linearly with u, so the tip moves along the segment "
                   "at constant speed.",
        .coefficients = preset_coefficients::line,
        .is_3d = false,
    },
    CurvePreset{
        .name = "Parabolic arc",
        .caption = "The u^2*a2 arrow starts tiny and grows fastest, bending the path back down. "
                   "The tip reaches the apex at u = 1/2.",
        .coefficients = preset_coefficients::parabolic_arc,
        .is_3d = false,
    },
    CurvePreset{
        .name = "Twisted cubic",
        .caption = "Each arrow adds a new direction: u*a1 along x, u^2*a2 along y, u^3*a3 "
                   "along z. With three independent directions, the curve leaves every plane.",
        .coefficients = preset_coefficients::twisted_cubic,
        .is_3d = true,
    },
};
