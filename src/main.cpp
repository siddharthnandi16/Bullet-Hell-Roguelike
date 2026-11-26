// main.cpp
#include "raylib.h"

enum class Screen { Title, Playing };

// Button is laid out in screen space, centered on the window.
Rectangle StartButtonRect(int winW, int winH) {
    const float w = 240.0f, h = 56.0f;
    return { (winW - w) / 2.0f, winH * 0.6f, w, h };
}

void DrawTitle(int winW, int winH, Rectangle button, bool hovered) {
    ClearBackground(BLACK);

    const char* title = "BULLET-HELL ROGUELIKE";
    int titleSize = 40;
    DrawText(title, (winW - MeasureText(title, titleSize)) / 2, winH * 0.3f,
             titleSize, { 80, 200, 255, 255 });

    Color fill = hovered ? Color{ 40, 90, 120, 255 } : Color{ 20, 50, 70, 255 };
    DrawRectangleRec(button, fill);
    DrawRectangleLinesEx(button, 2.0f, { 80, 200, 255, 255 });

    const char* label = "START GAME";
    int labelSize = 28;
    DrawText(label,
             (int)(button.x + (button.width - MeasureText(label, labelSize)) / 2),
             (int)(button.y + (button.height - labelSize) / 2),
             labelSize, WHITE);
}
// Logic dimensions (Touhou playfield, 6:7). One unit = one original Touhou pixel.
constexpr float kPlayW = 384.0f;
constexpr float kPlayH = 448.0f;

// Window and HUD sizes in screen pixels.
constexpr int kWindowH = 721;
constexpr int kHudW    = 240;   // reserved space on the right for the HUD
constexpr int kWindowW = 618 + kHudW; // initial width; playfield is 540 px tall-scaled

struct Layout {
    float scale;     // screen pixels per logic unit
    Rectangle play;  // playfield rectangle on screen
    Rectangle hud;   // HUD rectangle on screen
};

// Compute the largest scale that fits the playfield height and leaves kHudW on the right.
Layout ComputeLayout(int winW, int winH) {
    float scaleByH = winH / kPlayH;
    float scaleByW = (winW - kHudW) / kPlayW;
    float scale = (scaleByH < scaleByW) ? scaleByH : scaleByW;
    if (scale < 1.0f) scale = 1.0f;

    float playW = kPlayW * scale;
    float playH = kPlayH * scale;

    Layout l;
    l.scale = scale;
    l.play = { 0.0f, 0.0f, playW, playH };
    l.hud  = { playW, 0.0f, (float)winW - playW, (float)winH };
    return l;
}

// Convert a logic-unit position to screen pixels.
Vector2 ToScreen(const Layout& l, float x, float y) {
    return { l.play.x + x * l.scale, l.play.y + y * l.scale };
}

void DrawPlayfieldFrame(const Layout& l) {
    DrawRectangleRec(l.play, { 0, 0, 0, 255 });          // playfield background
    DrawRectangleLinesEx(l.play, 2.0f, { 80, 200, 255, 255 }); // border
}

void DrawHudFrame(const Layout& l) {
    DrawRectangleRec(l.hud, { 20, 20, 30, 255 });            // HUD background, empty for now
    DrawLine((int)l.hud.x, 0, (int)l.hud.x, (int)l.hud.height,
             { 80, 200, 255, 255 });                         // separator
}
//Main gameloop, define all functions above it
int main() {
    InitWindow(858, 721, "Bullet-Hell Roguelike");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    Screen screen = Screen::Title;

    while (!WindowShouldClose()) {
        int winW = GetScreenWidth();
        int winH = GetScreenHeight();
        Rectangle button = StartButtonRect(winW, winH);
        bool hovered = CheckCollisionPointRec(GetMousePosition(), button);

        // Title input: Enter or click starts the game.
        if (screen == Screen::Title) {
            bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            if (IsKeyPressed(KEY_ENTER) || clicked) {
                screen = Screen::Playing;
            }
        }

        BeginDrawing();
        if (screen == Screen::Title) {
            DrawTitle(winW, winH, button, hovered);
        } else {
            Layout l = ComputeLayout(winW, winH);
            ClearBackground(BLACK);
            DrawPlayfieldFrame(l);
            DrawHudFrame(l);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}