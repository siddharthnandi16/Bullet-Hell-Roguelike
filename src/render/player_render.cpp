// src/render/player_render.cpp
#include "raylib.h"
#include "gameplay/player.h"
#include "player_render.h"
#include <algorithm>

void draw_player(const Player& player, int window_w, int window_h) {
    const float logic_w = 384.0f;
    const float logic_h = 448.0f;

    float scale = std::min(window_w / logic_w, window_h / logic_h);
    float offset_x = (window_w - logic_w * scale) / 2.0f;
    float offset_y = (window_h - logic_h * scale) / 2.0f;

    int screen_x = static_cast<int>(offset_x + player.x() * scale);
    int screen_y = static_cast<int>(offset_y + player.y() * scale);

    const int font_size = static_cast<int>(20 * scale);
    const char* glyph = "@";

    // Center the glyph on the position rather than drawing from its top-left corner
    int text_w = MeasureText(glyph, font_size);
    DrawText(glyph, screen_x - text_w / 2, screen_y - font_size / 2, font_size, GREEN);
    float r = std::max(player.get_width(), player.get_height()) * 0.5f * scale;   // requires width()/height() getters
    DrawCircle(screen_x, screen_y, r, RED);
}