#pragma once

#include <string>

// window and view
const int WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 980;
const float VIEW_BOUNDS[] = { -11.0f, 12.0f, -11.0f, 12.0f };

// maze parameters
const int MAZE_ROWS = 19, MAZE_COLS = 19;
const int WALLS_TO_REMOVE = 50;

// text font path
const char* const ARIAL_TTF = "Fonts/arial.ttf";

// image file paths
const std::string CYBER_TEX = "Textures/cyber.jpg";
const std::string START_TEX = "Textures/start_tex.jpg";
const std::string END_TEX = "Textures/end_tex.jpg";
const std::string BRICK_TEX = "Textures/brick.png";
const std::string LAVA_TEX = "Textures/lava.png";

// shader paths
const char* const DEFAULT_FS = "Shaders/DefaultFragShader.fs";
const char* const DEFAULT_VS = "Shaders/DefaultVertShader.vs";
const char* const TEXTURE_FS = "Shaders/TextureFragShader.fs";
const char* const TEXTURE_VS = "Shaders/TextureVertShader.vs";
const char* const FTTEXT_FS = "Shaders/FTTextFragShader.fs";
const char* const FTTEXT_VS = "Shaders/FTTextVertShader.vs";