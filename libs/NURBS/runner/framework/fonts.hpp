/// @file
/// @brief The NURBS runner's text font.

#pragma once

#include <span>

#include <raylib.h>

namespace milt::runner {

/// @brief The bytes of `assets/NotoSans-Regular.ttf`, compiled in by `milt_embed_file`.
extern const std::span<const unsigned char> noto_sans_regular;

/// @brief The size, in pixels, that ui_font() is loaded at. Draw it at this size to keep it
/// sharp.
inline constexpr int ui_font_size = 18;

/// @brief Returns Noto Sans Regular at ui_font_size, loading it on the first call.
///
/// The first call also makes it raygui's font and sets raygui's text size to match. It holds
/// printable ASCII plus the characters for writing terms like u²·a₂: superscripts ² and ³,
/// the middle dot and subscripts ₀ to ₉. Other characters draw as '?'.
///
/// The font is never unloaded; its texture goes when the window closes.
///
/// @pre The window is open, so call it from the frame passed to run_app.
///
/// @return The font.
[[nodiscard]] const Font& ui_font();

} // namespace milt::runner
