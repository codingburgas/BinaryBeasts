#pragma once
#include "data.h"
#include <vector>
#include "raylib.h"

// Draw a single team card at the given y position. Returns the height used.
int DrawTeamCard(const Team& team, int y, int cardWidth, int cardHeight, int nameX, int pointsX, int goalsX, int editX, bool showEdit, int fontSize, Vector2 mouse, bool* editClicked);

// Draw all team cards, returns the total height used.
int DrawTeamList(const std::vector<Team>& teams, int startY, int cardWidth, int cardHeight, int nameX, int pointsX, int goalsX, int editX, int fontSize, Vector2 mouse, int* editIdxClicked);
