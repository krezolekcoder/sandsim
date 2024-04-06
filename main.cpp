/*******************************************************************************************
 *
 *   raylib [core] example - Keyboard input
 *
 *   Example originally created with raylib 1.0, last time updated with
 *raylib 1.0
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows static linking with closed source
 *software
 *
 *   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdint.h>

#define SCREEN_WIDTH_BLOCK_CNT (64U)
#define SCREEN_HEIGHT_BLOCK_CNT (64U)
#define BLOCK_SIZE (8U)
#define SCREEN_WIDTH (BLOCK_SIZE * SCREEN_WIDTH_BLOCK_CNT)
#define SCREEN_HEIGHT (BLOCK_SIZE * SCREEN_HEIGHT_BLOCK_CNT)

typedef enum {
  UP = 0,
  DOWN = 1,
} hueColorChangeState;

static void GridDraw(void);
static void SandSimulationStep(void);

static uint16_t grid[SCREEN_WIDTH_BLOCK_CNT][SCREEN_HEIGHT_BLOCK_CNT];
static uint16_t hue;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
             "raylib [core] example - keyboard input");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
  Vector2 mousePosition = {-100.0f, 100.0f};
  // auto simulationTimerMs = std::chrono::high_resolution_clock::now();
  float hueChangeSpeed = 100.0f;
  hueColorChangeState hueChState = UP;

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------
    mousePosition = GetMousePosition();

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // auto currentTimeMs = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
    //     currentTimeMs - simulationTimerMs);
    //
    switch (hueChState) {
    case UP:
      hue += static_cast<uint16_t>(hueChangeSpeed * GetFrameTime());
      if (hue >= 360U) {
        hueChState = DOWN;
      }
      break;

    case DOWN:
      hue -= static_cast<uint16_t>(hueChangeSpeed * GetFrameTime());
      if (hue <= 0U) {
        hueChState = UP;
      }
      break;

    default:
      break;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      uint32_t x = (uint32_t)mousePosition.x / BLOCK_SIZE;
      uint32_t y = (uint32_t)mousePosition.y / BLOCK_SIZE;

      grid[x][y] = hue;
      if (x - 1 >= 0) {
        grid[x - 1][y] = hue;
      } else if (x + 1 < SCREEN_WIDTH_BLOCK_CNT) {
        grid[x + 1][y] = hue;
      }
      if (y + 1 < SCREEN_HEIGHT_BLOCK_CNT) {
        grid[x][y + 1] = hue;
      } else if (y - 1 >= 0) {
        grid[x][y - 1] = hue;
      }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      for (int x = 0; x < SCREEN_WIDTH_BLOCK_CNT; x++) {
        for (int y = 0; y < SCREEN_HEIGHT_BLOCK_CNT; y++) {
          grid[x][y] = 0U;
        }
      }
    }

    SandSimulationStep();
    GridDraw();
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

static void GridDraw(void) {

  for (int x = 0; x < SCREEN_WIDTH_BLOCK_CNT; x++) {
    for (int y = 0; y < SCREEN_HEIGHT_BLOCK_CNT; y++) {
      Color color = WHITE;
      uint16_t hue = grid[x][y];

      if (hue != 0U) {
        color = ColorFromHSV(static_cast<float>(hue), 0.8f, 0.9f);
      }
      DrawRectangle(BLOCK_SIZE * x, BLOCK_SIZE * y, BLOCK_SIZE, BLOCK_SIZE,
                    color);

      DrawRectangleLines(BLOCK_SIZE * x, BLOCK_SIZE * y, BLOCK_SIZE, BLOCK_SIZE,
                         LIGHTGRAY);
    }
  }
}

static uint16_t swapGrid[SCREEN_WIDTH_BLOCK_CNT][SCREEN_HEIGHT_BLOCK_CNT];

static void SandSimulationStep(void) {

  for (int x = 0; x < SCREEN_WIDTH_BLOCK_CNT; x++) {
    for (int y = 0; y < SCREEN_HEIGHT_BLOCK_CNT; y++) {
      uint32_t state = grid[x][y];
      uint32_t below = grid[x][y + 1];

      srand(time(nullptr));
      int randomNum = rand() % 2;
      int dir = randomNum == 0 ? 1 : -1;

      uint32_t belowL = grid[x - dir][y + 1];
      uint32_t belowR = grid[x + dir][y + 1];

      if (y + 1 < SCREEN_HEIGHT_BLOCK_CNT && y - 1 > 0 &&
          x + 1 < SCREEN_WIDTH_BLOCK_CNT && x - 1 > 0) {
        if (state != 0U) {
          if (below == 0U) {
            swapGrid[x][y] = 0U;
            swapGrid[x][y + 1] = state;
          } else if (belowL == 0U) {
            swapGrid[x][y] = 0U;
            swapGrid[x - dir][y + 1] = state;
          } else if (belowR == 0U) {
            swapGrid[x][y] = 0U;
            swapGrid[x + dir][y + 1] = state;
          } else {
            swapGrid[x][y] = state;
          }
        }
      }
    }
  }

  // update grid with swap
  for (int x = 0; x < SCREEN_WIDTH_BLOCK_CNT; x++) {
    for (int y = 0; y < SCREEN_HEIGHT_BLOCK_CNT; y++) {
      grid[x][y] = swapGrid[x][y];
    }
  }
}
