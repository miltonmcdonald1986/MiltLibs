#include "fonts.hpp"

#include <span>
#include <vector>

#include <raygui.h>
#include <raylib.h>

namespace milt::runner {

// The bytes of assets/NotoSans-Regular.ttf, compiled in by milt_embed_file. Defined in the
// generated source, so it can't go in the anonymous namespace.
extern const std::span<const unsigned char> noto_sans_regular;

namespace {

// Printable ASCII, then the extra characters ui_font() promises.
std::vector<int> ui_codepoints() {
    std::vector<int> codepoints;
    for (int c = ' '; c <= '~'; ++c) {
        codepoints.push_back(c);
    }
    codepoints.push_back(0x00B2);            // ²
    codepoints.push_back(0x00B3);            // ³
    codepoints.push_back(0x00B7);            // ·
    for (int c = 0x2080; c <= 0x2089; ++c) { // ₀ to ₉
        codepoints.push_back(c);
    }
    return codepoints;
}

Font load_ui_font() {
    std::vector<int> codepoints = ui_codepoints();
    const Font font = LoadFontFromMemory(".ttf", noto_sans_regular.data(),
                                         static_cast<int>(noto_sans_regular.size()), ui_font_size,
                                         codepoints.data(), static_cast<int>(codepoints.size()));
    // Text drawn at a fractional position samples between texels; bilinear keeps it smooth.
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, ui_font_size);
    return font;
}

} // namespace

const Font& ui_font() {
    static const Font font = load_ui_font();
    return font;
}

} // namespace milt::runner
