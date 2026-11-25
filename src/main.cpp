#include "raylib.h"
#include "cJSON.h"      // JSON loading (used later)
#include "miniaudio.h"  // Audio (used later)

// NOTE: MINIAUDIO_IMPLEMENTATION is not defined here. raylib already bundles
// miniaudio internally, so defining it again would cause duplicate symbols.

int main() {
    const int screenWidth = 960;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Bullet-Hell Roguelike");
    SetTargetFPS(60);

    enum class Screen { Title, Gameplay };
    Screen current = Screen::Title;

    const char* title = "start game";
    const int fontSize = 40;
    const int textX = (screenWidth - MeasureText(title, fontSize)) / 2;
    const int textY = (screenHeight - fontSize) / 2;

    while (!WindowShouldClose()) {
        if (current == Screen::Title && IsKeyPressed(KEY_ENTER)) {
            current = Screen::Gameplay;
        }

        BeginDrawing();
        if (current == Screen::Title) {
            ClearBackground(RAYWHITE);
            DrawText(title, textX, textY, fontSize, DARKGRAY);
        } else {
            ClearBackground(BLACK);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
