#include "presentation.h"
#include "raylib.h"
#include "logic.h"
#include <ctime>
#include <cstdlib>

void run_app() {
    InitWindow(900, 600, "Sports Manager Pro - FINAL VERSION");
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));

    std::vector<Team> teams = load_data();

    // Начални данни, ако файлът е празен
    if (teams.empty()) {
        add_team_logic(teams, "Real Madrid");
        add_team_logic(teams, "FC Barcelona");
        add_team_logic(teams, "Manchester City");
    }

    std::vector<Match> matches = { {2, 1}, {3, 2}, {0, 1} };
    char inputBuf[50] = "\0";
    int letterCount = 0;

    while (!WindowShouldClose()) {
        int key = GetCharPressed();
        while (key > 0) {
            if (letterCount < 49 && key >= 32 && key <= 125) {
                inputBuf[letterCount] = (char)key;
                inputBuf[letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) inputBuf[--letterCount] = '\0';
        if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
            add_team_logic(teams, inputBuf);
            inputBuf[0] = '\0'; letterCount = 0;
        }

        if (IsKeyPressed(KEY_F1) && teams.size() > 1) quick_sort_teams(teams, 0, (int)teams.size() - 1);
        if (IsKeyPressed(KEY_F2)) delete_last_team_logic(teams);
        if (IsKeyPressed(KEY_F3)) clear_all_teams_logic(teams);

        BeginDrawing();
        ClearBackground(GetColor(0x111111FF));

        DrawRectangle(0, 0, 900, 80, DARKBLUE);
        DrawText("LEAGUE STANDINGS", 20, 20, 30, WHITE);
        DrawText("[F1] QuickSort | [F2] Del Last | [F3] Clear All", 380, 25, 17, LIGHTGRAY);

        for (int i = 0; i < (int)teams.size(); i++) {
            int y = 100 + (i * 45);
            DrawRectangle(20, y, 860, 40, (i % 2 == 0) ? GetColor(0x222222FF) : GetColor(0x2D2D2DFF));
            DrawText(teams[i].name, 40, y + 10, 20, RAYWHITE);
            DrawText(TextFormat("%d pts", teams[i].points), 400, y + 10, 20, GOLD);
        }

        DrawRectangle(0, 540, 900, 60, BLACK);
        int totalGoals = calculate_total_goals_recursive(matches, (int)matches.size());
        DrawText(TextFormat("Recursive Total Goals: %d", totalGoals), 20, 560, 20, GREEN);
        DrawText(TextFormat("New Team: %s", inputBuf), 600, 560, 20, SKYBLUE);

        EndDrawing();
    }
    save_data(teams);
    CloseWindow();
}