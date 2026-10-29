#include "horner_view.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <format>
#include <string>
#include <vector>

#include <glm/vec3.hpp>
#include <raygui.h>
#include <raylib.h>

#include <milt/nurbs/power_basis.hpp>

#include "framework/draw3d.hpp"
#include "framework/fonts.hpp"
#include "framework/raylib_convert.hpp"
#include "horner_model.hpp"
#include "horner_presets.hpp"

namespace {

// u values sampled when fitting the view; 1/64 apart.
constexpr std::size_t bounds_samples = 65;

// Line segments in the curve's polyline; enough that no corners show.
constexpr int curve_segments = 200;

// Sizes in the scene, as fractions of Bounds::radius() so every preset looks alike.
constexpr double arrow_radius = 0.01;
constexpr double a0_radius = 0.02;
constexpr double marker_radius = 0.015;

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

// The color of the arrow for the degree-k term; a1 is blue, a2 orange, a3 and up green.
Color term_color(std::size_t k) {
    switch (k) {
    case 1:
        return BLUE;
    case 2:
        return ORANGE;
    default:
        return DARKGREEN;
    }
}

// The label for the degree-k term, such as u²·a₂. ui_font() has no superscripts past ³, so
// higher degrees fall back to ASCII.
std::string term_label(std::size_t k) {
    switch (k) {
    case 1:
        return "u·a₁";
    case 2:
        return "u²·a₂";
    case 3:
        return "u³·a₃";
    default:
        return std::format("u^{}·a_{}", k, k);
    }
}

} // namespace

Scene make_scene(const CurvePreset& preset, double aspect) {
    const Bounds bounds = fit_bounds(preset.coefficients, bounds_samples);
    return {.preset = preset, .bounds = bounds, .camera = top_down_camera(bounds, aspect)};
}

void draw_scene(const Scene& scene, double u) {
    using milt::nurbs::horner1;
    using milt::runner::draw_arrow_3d;
    using milt::runner::to_vector3;

    const auto coefficients = scene.preset.coefficients;
    const double size = scene.bounds.radius();

    DrawGrid(16, 0.25f);

    glm::dvec3 previous = horner1(coefficients, 0.0);
    for (int i = 1; i <= curve_segments; ++i) {
        const glm::dvec3 p = horner1(coefficients, i / static_cast<double>(curve_segments));
        DrawLine3D(to_vector3(previous), to_vector3(p), DARKGRAY);
        previous = p;
    }

    const std::vector<glm::dvec3> chain = term_chain(coefficients, u);
    assert(chain.back() == horner1(coefficients, u));

    DrawSphere(to_vector3(chain.front()), static_cast<float>(a0_radius * size), BLACK);
    for (std::size_t k = 1; k < chain.size(); ++k) {
        draw_arrow_3d(chain[k - 1], chain[k], arrow_radius * size, term_color(k));
    }

    // Only a little wider than an arrow's head, so it covers just the last head's point.
    DrawSphere(to_vector3(chain.back()), static_cast<float>(marker_radius * size), MAROON);
}

void draw_labels(const Scene& scene, double u) {
    using milt::runner::to_vector3;

    const Font& font = milt::runner::ui_font();
    constexpr auto font_size = static_cast<float>(milt::runner::ui_font_size);
    // Glyph spacing, in pixels, on top of the font's own.
    constexpr float spacing = 0.0f;
    // The gap between an arrow and the nearest edge of its label, in pixels.
    constexpr float label_gap = 6.0f;
    // Arrows shorter than this on screen get no label, so that near u = 0, where the chain
    // collapses onto a0, the labels don't pile up.
    constexpr float min_labeled_length = 12.0f;

    const std::vector<glm::dvec3> chain = term_chain(scene.preset.coefficients, u);
    for (std::size_t k = 1; k < chain.size(); ++k) {
        const Vector2 from = GetWorldToScreen(to_vector3(chain[k - 1]), scene.camera);
        const Vector2 to = GetWorldToScreen(to_vector3(chain[k]), scene.camera);
        const float dx = to.x - from.x;
        const float dy = to.y - from.y;
        const float length = std::hypot(dx, dy);
        if (length < min_labeled_length) {
            continue;
        }

        const Vector2 mid{.x = (from.x + to.x) / 2.0f, .y = (from.y + to.y) / 2.0f};
        // The arrow's screen-space normal, turned away from the previous arrow's tail so the
        // label doesn't land on that arrow.
        Vector2 normal{.x = -dy / length, .y = dx / length};
        if (k >= 2) {
            const Vector2 back = GetWorldToScreen(to_vector3(chain[k - 2]), scene.camera);
            if (normal.x * (back.x - mid.x) + normal.y * (back.y - mid.y) > 0.0f) {
                normal = {.x = -normal.x, .y = -normal.y};
            }
        }

        const std::string label = term_label(k);
        const Vector2 extent = MeasureTextEx(font, label.c_str(), font_size, spacing);
        // Far enough along the normal that the label's box clears the arrow.
        const float offset =
            label_gap + std::abs(normal.x) * extent.x / 2.0f + std::abs(normal.y) * extent.y / 2.0f;
        // Whole pixels, so the glyphs land on texels and stay sharp.
        const Vector2 corner{.x = std::round(mid.x + normal.x * offset - extent.x / 2.0f),
                             .y = std::round(mid.y + normal.y * offset - extent.y / 2.0f)};
        DrawTextEx(font, label.c_str(), corner, font_size, spacing, term_color(k));
    }
}

ViewState draw_controls(const CurvePreset& preset, ViewState current) {
    // Loading the font makes it raygui's, in case nothing has drawn text yet.
    static_cast<void>(milt::runner::ui_font());

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
