#include "horner_view.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <string>

#include <glm/vec3.hpp>
#include <raygui.h>
#include <raylib.h>

#include <milt/nurbs/power_basis.hpp>

#include "framework/raylib_convert.hpp"
#include "horner_model.hpp"
#include "horner_presets.hpp"

namespace {

// u values sampled when fitting the view; 1/64 apart.
constexpr std::size_t bounds_samples = 65;

// Line segments in the curve's polyline; enough that no corners show.
constexpr int curve_segments = 200;

Camera3D top_down_camera(const Bounds& bounds, double aspect) {
    using milt::runner::to_vector3;

    // Extra room around the bounds, as a factor of their size.
    constexpr double margin = 1.3;

    const glm::dvec3 center = bounds.center();
    const glm::dvec3 size = bounds.max - bounds.min;
    // An orthographic camera's fovy is the visible height in world units.
    const double height = std::max(size.y, size.x / aspect) * margin;
    // Far enough above the bounds that none of it is behind the near plane.
    const glm::dvec3 eye = center + glm::dvec3{0.0, 0.0, bounds.radius() + 1.0};

    return {
        .position = to_vector3(eye),
        .target = to_vector3(center),
        .up = to_vector3({0.0, 1.0, 0.0}),
        .fovy = static_cast<float>(height),
        .projection = CAMERA_ORTHOGRAPHIC,
    };
}

} // namespace

Scene make_scene(const CurvePreset& preset, double aspect) {
    const Bounds bounds = fit_bounds(preset.coefficients, bounds_samples);
    return {.preset = preset, .bounds = bounds, .camera = top_down_camera(bounds, aspect)};
}

void draw_scene(const Scene& scene, double u) {
    using milt::nurbs::horner1;
    using milt::runner::to_vector3;

    const auto coefficients = scene.preset.coefficients;

    DrawGrid(16, 0.25f);

    glm::dvec3 previous = horner1(coefficients, 0.0);
    for (int i = 1; i <= curve_segments; ++i) {
        const glm::dvec3 p = horner1(coefficients, i / static_cast<double>(curve_segments));
        DrawLine3D(to_vector3(previous), to_vector3(p), DARKGRAY);
        previous = p;
    }

    const auto marker_radius = static_cast<float>(0.03 * scene.bounds.radius());
    DrawSphere(to_vector3(horner1(coefficients, u)), marker_radius, MAROON);
}

ViewState draw_controls(const CurvePreset& preset, ViewState current) {
    // raygui needs a null-terminated title, which a string_view doesn't promise.
    const std::string title{preset.name};
    GuiGroupBox({.x = 10, .y = 10, .width = 230, .height = 50}, title.c_str());

    // raygui sliders work in float.
    auto u = static_cast<float>(current.u);
    const std::string label = std::format("{:.2f}", current.u);
    GuiSliderBar({.x = 40, .y = 25, .width = 150, .height = 20}, "u", label.c_str(), &u, 0.0f,
                 1.0f);
    current.u = u;

    return current;
}
