#pragma once

#include "data.h"
#include "raylib.h"
#include <vector>

enum class AppScreen {
    MainMenu,
    LeagueStandings,
    LiveScores,
    MatchSchedule,
    SeasonStats,
    About,
    Quit,
};

// Main menu: returns target screen when a tile is clicked, else MainMenu.
AppScreen UpdateAndDrawMainMenu(Vector2 mouse, int screenW, int screenH);

void DrawLiveScoresScreen(const std::vector<Team>& teams, Vector2 mouse, int screenW, int screenH, bool* goBack);
void DrawMatchScheduleScreen(const std::vector<Team>& teams, Vector2 mouse, int screenW, int screenH, bool* goBack);
void DrawSeasonStatsScreen(const std::vector<Team>& teams, Vector2 mouse, int screenW, int screenH, bool* goBack);
void DrawAboutScreen(Vector2 mouse, int screenW, int screenH, bool* goBack);

void DrawSubScreenChrome(const char* title, Vector2 mouse, int screenW, int screenH, bool* goBack);
