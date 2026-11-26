# Code Documentation

## Overview

A single `main.cpp` with a title screen and a playing screen. The title screen has a START GAME button that switches to the playfield view. Nothing moves yet.

## Constants

- `kPlayW`, `kPlayH`: playfield size in logic units, 384 × 448 (Touhou's 6:7 playfield). One unit is one original Touhou pixel.
- `kWindowH`, `kHudW`: initial window height and the HUD width in screen pixels.
- `kWindowW`: initial window width, computed from the two values above.

## Layout

`struct Layout` holds:

- `scale`: screen pixels per logic unit.
- `play`: the playfield rectangle on screen.
- `hud`: the HUD rectangle on screen, to the right of the playfield.

`ComputeLayout(winW, winH)` picks the largest scale that fits the playfield height and leaves `kHudW` for the HUD. It clamps the scale to at least 1.0. The playfield is anchored at the top-left.

`ToScreen(layout, x, y)` converts logic coordinates to screen pixels. Use it only when drawing.

## Drawing

- `DrawPlayfieldFrame(layout)`: fills the playfield with a dark background and draws a blue border.
- `DrawHudFrame(layout)`: fills the HUD area and draws a separator line. The HUD is empty for now.
- `DrawTitle(winW, winH, button, hovered)`: draws the title text and the START GAME button, centered on the window.

## Title Screen

- `enum class Screen { Title, Playing }` tracks the current screen.
- `StartButtonRect(winW, winH)` returns the button rectangle in screen space.
- Enter or a left click on the button switches to `Playing`.
- There is no way back to the title screen yet.

## Main Loop

1. Read the window size and compute the button rectangle and hover state.
2. On the title screen, check for Enter or a click and switch to `Playing` if pressed.
3. Draw the current screen. The playing screen computes the layout and draws the playfield and HUD frames.

## Known Limitations

- Gameplay logic, player input, and weapons are not implemented.
- The window's initial size (858 × 721) is the smallest size with no unused space at the current layout. Other sizes show a gap.
- Resizing rescales drawing only. Gameplay positions stay in logic units.
