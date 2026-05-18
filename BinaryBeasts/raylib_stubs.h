#pragma once

// Minimal raylib types and constants for compilation without raylib.h

struct Color { unsigned char r, g, b, a; };
struct Vector2 { float x, y; };
struct Rectangle { float x, y, width, height; };

// Color constants
static const Color WHITE = {255,255,255,255};
static const Color BLACK = {0,0,0,255};

// Key constants
#define KEY_NULL 0
#define KEY_ESCAPE 256
#define KEY_ENTER 257
#define KEY_BACKSPACE 259
#define KEY_DELETE 261
#define KEY_SLASH 47
#define KEY_F1 290
#define KEY_F2 291
#define KEY_F3 292
#define KEY_UP 265
#define KEY_DOWN 264

// Mouse constants
#define MOUSE_LEFT_BUTTON 0

// Dummy function declarations for build (replace with real raylib or mocks for testing)
int IsKeyPressed(int key);
int IsKeyPressedRepeat(int key);
int IsMouseButtonPressed(int button);
int CheckCollisionPointRec(Vector2 point, Rectangle rec);
int MeasureText(const char* text, int fontSize);
int GetCharPressed();
inline int GetFrameCount() { static int f = 0; return ++f; }
Vector2 GetMousePosition();
void DrawText(const char* text, int posX, int posY, int fontSize, Color color);
void DrawRectangle(int posX, int posY, int width, int height, Color color);
void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);
void DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color);
void DrawCircle(int centerX, int centerY, float radius, Color color);
void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);
void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
Color GetColor(unsigned int hexValue);
Color Fade(Color color, float alpha);
void ClearBackground(Color color);
void BeginDrawing();
void EndDrawing();
void InitWindow(int width, int height, const char* title);
void SetTargetFPS(int fps);
void SetExitKey(int key);
int WindowShouldClose();
void CloseWindow();
char* TextFormat(const char* text, ...);
