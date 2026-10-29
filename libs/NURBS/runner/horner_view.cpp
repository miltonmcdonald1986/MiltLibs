#include "horner_view.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <format>
#include <numbers>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
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

// The direction a 3D preset is seen from: swung this far from the -y side toward +x, and
// raised this far above the x-y plane.
constexpr double oblique_yaw = 35.0 * std::numbers::pi / 180.0;
constexpr double oblique_pitch = 25.0 * std::numbers::pi / 180.0;

// The control panel's layout, in pixels. The panel is a column down the left of the screen,
// and the scene is framed in the rest.
constexpr float panel_margin = 10.0f;  // Between the panel's box and the window's edge.
constexpr float panel_padding = 10.0f; // Between the panel's box and its contents.
constexpr float panel_content_width = 340.0f;
constexpr float panel_width = 2.0f * (panel_margin + panel_padding) + panel_content_width;
constexpr float control_height = 28.0f; // The preset picker and the u slider.
constexpr float line_height = 22.0f;    // Lines of the caption and coefficients.
constexpr float section_gap = 12.0f;    // Between the panel's sections.

// Text options shared by everything drawn with ui_font().
constexpr auto font_size = static_cast<float>(milt::runner::ui_font_size);
constexpr float spacing = 0.0f; // Glyph spacing, in pixels, on top of the font's own.

// An orthographic camera looking at the bounds from `toward_eye`, a unit vector, with `up`
// pointing up the screen. It frames `extent`, the world width and height to show, in the part
// of the screen right of the panel.
Camera3D framing_camera(const Bounds& bounds, glm::dvec3 toward_eye, glm::dvec3 up,
                        glm::dvec2 extent, ScreenSize screen) {
    using milt::runner::to_vector3;

    const double aspect = (screen.width - double{panel_width}) / screen.height;
    // An orthographic camera's fovy is the visible height in world units.
    const double fovy = std::max(extent.y, extent.x / aspect);

    // Shift the camera left by half the panel's width, so the bounds' center lands in the
    // middle of the area right of the panel instead of mid-screen.
    const glm::dvec3 right = glm::normalize(glm::cross(up, toward_eye));
    const double units_per_pixel = fovy / screen.height;
    const glm::dvec3 target = bounds.center() - right * (panel_width / 2.0 * units_per_pixel);
    // The shift is across the view, so the bounds still lie within radius() of the target
    // along it, and this far out none of them are behind the near plane.
    const glm::dvec3 eye = target + toward_eye * (bounds.radius() + 1.0);

    return {
        .position = to_vector3(eye),
        .target = to_vector3(target),
        .up = to_vector3(up),
        .fovy = static_cast<float>(fovy),
        .projection = CAMERA_ORTHOGRAPHIC,
    };
}

// Looks straight down the math z axis and fits the bounds' x-y extent.
Camera3D top_down_camera(const Bounds& bounds, ScreenSize screen) {
    // Extra room around the bounds, as a factor of their size.
    constexpr double margin = 1.3;

    const glm::dvec2 size{bounds.max - bounds.min};
    return framing_camera(bounds, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, size * margin, screen);
}

// Looks from oblique_yaw and oblique_pitch and fits the sphere around the bounds, so the
// bounds fit from any angle.
Camera3D oblique_camera(const Bounds& bounds, ScreenSize screen) {
    // Extra room around the sphere, as a factor of its size. The sphere is already looser
    // than the bounds, so it needs less than top_down_camera.
    constexpr double margin = 1.1;

    const glm::dvec3 toward_eye{std::sin(oblique_yaw) * std::cos(oblique_pitch),
                                -std::cos(oblique_yaw) * std::cos(oblique_pitch),
                                std::sin(oblique_pitch)};
    const glm::dvec2 diameter{2.0 * bounds.radius()};
    return framing_camera(bounds, toward_eye, {0.0, 0.0, 1.0}, diameter * margin, screen);
}

// The color of a0's dot and of each term's arrow: a0 black, a1 blue, a2 orange, a3 and up
// green.
Color term_color(std::size_t k) {
    switch (k) {
    case 0:
        return BLACK;
    case 1:
        return BLUE;
    case 2:
        return ORANGE;
    default:
        return DARKGREEN;
    }
}

// n written in subscript digits, such as ₁₂ for 12.
std::string subscript(std::size_t n) {
    std::string digits;
    for (const char digit : std::to_string(n)) {
        // In UTF-8, the subscript digits U+2080 to U+2089 are E2 82 80 to E2 82 89.
        digits += "\xE2\x82";
        digits += static_cast<char>(0x80 + (digit - '0'));
    }
    return digits;
}

// The label for the degree-k term, such as u²·a₂. ui_font() has no superscripts past ³, so
// higher powers fall back to ^.
std::string term_label(std::size_t k) {
    switch (k) {
    case 1:
        return "u·a₁";
    case 2:
        return "u²·a₂";
    case 3:
        return "u³·a₃";
    default:
        return std::format("u^{}·a{}", k, subscript(k));
    }
}

// The coefficient a_k as text, such as a₁ = (2, 1). A 2D preset's z components are all 0,
// so they're left out.
std::string coefficient_text(std::size_t k, glm::dvec3 a, bool is_3d) {
    return is_3d ? std::format("a{} = ({:g}, {:g}, {:g})", subscript(k), a.x, a.y, a.z)
                 : std::format("a{} = ({:g}, {:g})", subscript(k), a.x, a.y);
}

// Breaks text at spaces into lines no wider than `width` pixels in ui_font(). A word wider
// than `width` gets a line of its own.
std::vector<std::string> wrap_text(std::string_view text, float width) {
    const Font& font = milt::runner::ui_font();

    std::vector<std::string> lines;
    std::string line;
    for (const auto word_chars : std::views::split(text, ' ')) {
        const std::string_view word{word_chars.begin(), word_chars.end()};
        std::string longer = line.empty() ? std::string{word} : std::format("{} {}", line, word);
        if (!line.empty() && MeasureTextEx(font, longer.c_str(), font_size, spacing).x > width) {
            lines.push_back(std::move(line));
            line = word;
        } else {
            line = std::move(longer);
        }
    }
    if (!line.empty()) {
        lines.push_back(std::move(line));
    }
    return lines;
}

// The preset picker's options: the preset names, separated by semicolons.
std::string preset_names() {
    std::string names;
    for (const CurvePreset& preset : curve_presets) {
        if (!names.empty()) {
            names += ';';
        }
        names += preset.name;
    }
    return names;
}

} // namespace

Scene make_scene(std::size_t preset, ScreenSize screen) {
    assert(preset < curve_presets.size());
    const CurvePreset& chosen = curve_presets.at(preset);
    const Bounds bounds = fit_bounds(chosen.coefficients, bounds_samples);
    const Camera3D camera =
        chosen.is_3d ? oblique_camera(bounds, screen) : top_down_camera(bounds, screen);
    return {.preset = chosen, .bounds = bounds, .camera = camera};
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

    DrawSphere(to_vector3(chain.front()), static_cast<float>(a0_radius * size), term_color(0));
    for (std::size_t k = 1; k < chain.size(); ++k) {
        draw_arrow_3d(chain[k - 1], chain[k], arrow_radius * size, term_color(k));
    }

    // Only a little wider than an arrow's head, so it covers just the last head's point.
    DrawSphere(to_vector3(chain.back()), static_cast<float>(marker_radius * size), MAROON);
}

void draw_labels(const Scene& scene, double u) {
    using milt::runner::to_vector3;

    const Font& font = milt::runner::ui_font();
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
    // Loading the font also makes it raygui's, in case nothing has drawn text yet.
    const Font& font = milt::runner::ui_font();

    const std::vector<std::string> caption = wrap_text(preset.caption, panel_content_width);
    const auto coefficients = preset.coefficients;

    // The group box's title sits on its top edge, so the contents start below the title.
    constexpr float top = panel_margin + font_size;
    const float height = (top - panel_margin) + 2.0f * control_height + 3.0f * section_gap +
                         static_cast<float>(caption.size() + coefficients.size()) * line_height +
                         panel_padding;
    // A group box is only an outline, so fill it to keep the scene from showing through.
    const Rectangle box{.x = panel_margin,
                        .y = panel_margin,
                        .width = panel_width - 2.0f * panel_margin,
                        .height = height};
    DrawRectangleRec(box,
                     GetColor(static_cast<unsigned int>(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))));
    GuiGroupBox(box, "Curve");

    constexpr float left = panel_margin + panel_padding;
    float y = top;

    // GuiToggleGroup takes the size of one toggle and lays the rest out in a row.
    const auto toggle_count = static_cast<float>(curve_presets.size());
    const auto toggle_gap = static_cast<float>(GuiGetStyle(TOGGLE, GROUP_PADDING));
    const float toggle_width =
        (panel_content_width - (toggle_count - 1.0f) * toggle_gap) / toggle_count;
    auto active = static_cast<int>(current.preset);
    const std::string names = preset_names();
    GuiToggleGroup({.x = left, .y = y, .width = toggle_width, .height = control_height},
                   names.c_str(), &active);
    current.preset = static_cast<std::size_t>(active);
    y += control_height + section_gap;

    // Room for the slider's text: "u" on its left and the value, such as 0.40, on its right.
    constexpr float name_width = 20.0f;
    constexpr float value_width = 50.0f;
    // raygui sliders work in float.
    auto u = static_cast<float>(current.u);
    const std::string value = std::format("{:.2f}", current.u);
    GuiSliderBar({.x = left + name_width,
                  .y = y,
                  .width = panel_content_width - name_width - value_width,
                  .height = control_height},
                 "u", value.c_str(), &u, 0.0f, 1.0f);
    current.u = u;
    y += control_height + section_gap;

    for (const std::string& line : caption) {
        DrawTextEx(font, line.c_str(), {.x = left, .y = y}, font_size, spacing, DARKGRAY);
        y += line_height;
    }
    y += section_gap;

    for (std::size_t k = 0; k < coefficients.size(); ++k) {
        const std::string text = coefficient_text(k, coefficients[k], preset.is_3d);
        DrawTextEx(font, text.c_str(), {.x = left, .y = y}, font_size, spacing, term_color(k));
        y += line_height;
    }

    return current;
}
