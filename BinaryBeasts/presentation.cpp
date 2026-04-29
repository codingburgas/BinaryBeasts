#include "raylib.h"
#include "logic.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cstring>

static void DrawFittedText(const char* text, Rectangle box, int startFont, int minFont, Color color, bool centerAlign = true) {
    int fontSize = startFont;
    int textWidth = MeasureText(text, fontSize);
    while (textWidth > (int)box.width - 12 && fontSize > minFont) {
        fontSize--;
        textWidth = MeasureText(text, fontSize);
    }

    char buffer[128];
    strncpy_s(buffer, sizeof(buffer), text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    int len = (int)strlen(buffer);
    while (MeasureText(buffer, fontSize) > (int)box.width - 12 && len > 3) {
        buffer[--len] = '\0';
        int ellipIdx = (len >= 3) ? len - 3 : 0;
        strcpy_s(&buffer[ellipIdx], sizeof(buffer) - ellipIdx, "...");
    }

    int drawX = (int)box.x + 6;
    if (centerAlign) {
        drawX = (int)box.x + ((int)box.width - MeasureText(buffer, fontSize)) / 2;
    }
    int drawY = (int)box.y + ((int)box.height - fontSize) / 2;
    DrawText(buffer, drawX, drawY, fontSize, color);
}

static void DrawEllipsizedText(const char* text, Rectangle box, int fontSize, Color color, bool centerAlign = false) {
    char buffer[128];
    strncpy_s(buffer, sizeof(buffer), text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    int len = (int)strlen(buffer);
    while (MeasureText(buffer, fontSize) > (int)box.width - 10 && len > 3) {
        buffer[--len] = '\0';
        int ellipIdx = (len >= 3) ? len - 3 : 0;
        strcpy_s(&buffer[ellipIdx], sizeof(buffer) - ellipIdx, "...");
    }

    int drawX = (int)box.x + 5;
    if (centerAlign) {
        drawX = (int)box.x + ((int)box.width - MeasureText(buffer, fontSize)) / 2;
    }
    int drawY = (int)box.y + ((int)box.height - fontSize) / 2;
    DrawText(buffer, drawX, drawY, fontSize, color);
}

static const char* GetScoreTier(const Team& team) {
    if (team.points >= 18) return "Elite";
    if (team.points >= 10) return "Strong";
    if (team.points >= 4) return "Rising";
    return "Developing";
}

static void ApplySort(std::vector<Team>& teams, int sortMode) {
    if (sortMode == 0) {
        std::sort(teams.begin(), teams.end(), [](const Team& a, const Team& b) {
            if (a.points == b.points) return a.goalsScored > b.goalsScored;
            return a.points > b.points;
        });
    } else if (sortMode == 1) {
        std::sort(teams.begin(), teams.end(), [](const Team& a, const Team& b) {
            if (a.goalsScored == b.goalsScored) return a.points > b.points;
            return a.goalsScored > b.goalsScored;
        });
    } else {
        std::sort(teams.begin(), teams.end(), [](const Team& a, const Team& b) {
            return strcmp(a.name, b.name) < 0;
        });
    }
}

void run_app() {
    InitWindow(1200, 800, "Sports Manager Pro - FINAL VERSION");
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
    bool showAddModal = false;
    bool inputActive = false;
    // Edit modal state
    bool showEditModal = false;
    int editTeamIdx = -1;
    char editNameBuf[50] = "\0";
    int editGoals = 0;
    int editPoints = 0;
    bool editNameActive = false;
    bool editGoalsActive = false;
    bool editPointsActive = false;

    // Sort toggle state: 0 = by points, 1 = by goals, 2 = by name
    int sortMode = 0;
    const char* sortLabels[3] = {"Sort: Points", "Sort: Goals", "Sort: Name"};
    while (!WindowShouldClose()) {
        // Handle modal input
        if (showAddModal) {
            int key = GetCharPressed();
            while (key > 0) {
                if (inputActive && letterCount < 49 && key >= 32 && key <= 125) {
                    inputBuf[letterCount] = (char)key;
                    inputBuf[letterCount + 1] = '\0';
                    letterCount++;
                }
                key = GetCharPressed();
            }
            if (inputActive && IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) inputBuf[--letterCount] = '\0';
            if (inputActive && IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                // Validation: points and goals non-negative (default 0)
                int valid = 1;
                for (int i = 0; inputBuf[i]; ++i) {
                    if (inputBuf[i] < 32) valid = 0;
                }
                if (valid) {
                    Team t;
                    t.id = (int)teams.size() + 1;
                    strncpy_s(t.name, sizeof(t.name), inputBuf, 49); t.name[49] = '\0';
                    t.points = 0;
                    t.goalsScored = 0;
                    teams.push_back(t);
                }
                inputBuf[0] = '\0'; letterCount = 0;
                showAddModal = false;
                inputActive = false;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                showAddModal = false;
                inputActive = false;
            }
        } else {
            if (IsKeyPressed(KEY_F1) && teams.size() > 1) {
                sortMode = (sortMode + 1) % 3;
                ApplySort(teams, sortMode);
            }
            if (IsKeyPressed(KEY_F2)) delete_last_team_logic(teams);
            if (IsKeyPressed(KEY_F3)) clear_all_teams_logic(teams);
        }

        BeginDrawing();
        Color bg = GetColor(0xF3F4F6FF);
        Color sidebar = GetColor(0x1F2937FF);
        Color panel = GetColor(0xFAFAF9FF);
        Color cardTop = GetColor(0xFFFFFFFF);
        Color cardBottom = GetColor(0xF1F5F9FF);
        Color accent = GetColor(0x0EA5E9FF);
        Color accentSoft = GetColor(0xC4B5FDFF);
        Color textMain = GetColor(0x111827FF);
        Color textMuted = GetColor(0x6B7280FF);
        Color success = GetColor(0x16A34AFF);
        Color warning = GetColor(0xCA8A04FF);

        const int FONT_TITLE = 38;
        const int FONT_SECTION = 24;
        const int FONT_BODY = 22;
        const int FONT_SMALL = 18;
        const int FONT_META = 16;
        const int SIDEBAR_W = 240;
        const int MAIN_X = 260;
        const int MAIN_Y = 34;
        const int MAIN_W = 920;
        const int MAIN_H = 620;
        const int CARD_X = MAIN_X + 18;
        const int CARD_W = MAIN_W - 36;
        const int FOOTER_X = MAIN_X;
        const int FOOTER_Y = 672;
        const int FOOTER_W = MAIN_W;
        const int FOOTER_H = 110;

        ClearBackground(bg);

        // Sidebar
        DrawRectangle(0, 0, SIDEBAR_W, 800, sidebar);
        DrawText("SPORTS\nMANAGER", 44, 50, 30, accentSoft);

        // Sidebar buttons with icons and highlight
        int btnY = 180;
        int btnH = 64;
        int btnPad = 24;
        Color btnColor = GetColor(0x374151FF);
        Color btnHover = GetColor(0x4B5563FF);
        Vector2 mouse = GetMousePosition();

        struct BtnInfo { const char* label; int icon; };
        BtnInfo btns[4] = {
            {"Add Team", 0},
            {sortLabels[sortMode], 1},
            {"Delete Last", 2},
            {"Clear All", 3}
        };
        Rectangle btnRects[4];
        bool btnHovers[4];
        for (int b = 0; b < 4; b++) {
            btnRects[b] = Rectangle{50, (float)btnY + b * (btnH + btnPad), 160, (float)btnH};
            btnHovers[b] = CheckCollisionPointRec(mouse, btnRects[b]);
            // Highlight bar
            if (btnHovers[b]) DrawRectangle(36, (int)btnRects[b].y, 8, btnH, accent);
            // Button background
            DrawRectangleRounded(btnRects[b], 0.22f, 8, btnHovers[b] ? btnHover : btnColor);
            // Icon (simple shapes)
            int iconX = 64, iconY = (int)btnRects[b].y + 22;
            switch (b) {
                case 0: // Add (plus)
                    DrawRectangle(iconX+6, iconY+2, 12, 4, WHITE);
                    DrawRectangle(iconX+10, iconY-2, 4, 12, WHITE);
                    break;
                case 1: // Sort (arrows)
                    DrawTriangle(Vector2{(float)(iconX+6), (float)(iconY+12)}, Vector2{(float)(iconX+12), (float)(iconY+4)}, Vector2{(float)(iconX+18), (float)(iconY+12)}, WHITE);
                    DrawTriangle(Vector2{(float)(iconX+6), (float)(iconY+2)}, Vector2{(float)(iconX+12), (float)(iconY+10)}, Vector2{(float)(iconX+18), (float)(iconY+2)}, WHITE);
                    break;
                case 2: // Delete (trash)
                    DrawRectangle(iconX+6, iconY+4, 12, 8, WHITE);
                    DrawRectangle(iconX+8, iconY+2, 8, 3, WHITE);
                    break;
                case 3: // Clear (X)
                    DrawLine(iconX+6, iconY+4, iconX+18, iconY+16, WHITE);
                    DrawLine(iconX+18, iconY+4, iconX+6, iconY+16, WHITE);
                    break;
            }
            // Label
            DrawEllipsizedText(btns[b].label, Rectangle{98, btnRects[b].y + 12, 108, 40}, FONT_META, WHITE, false);
        }
        // Button actions
        if (!showAddModal && !showEditModal && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (btnHovers[0]) { showAddModal = true; inputActive = true; }
            if (btnHovers[1] && teams.size() > 1) {
                sortMode = (sortMode + 1) % 3;
                ApplySort(teams, sortMode);
            }
            if (btnHovers[2]) delete_last_team_logic(teams);
            if (btnHovers[3]) clear_all_teams_logic(teams);
        }

        // Main content area
        DrawRectangleRounded(Rectangle{(float)MAIN_X, (float)MAIN_Y, (float)MAIN_W, (float)MAIN_H}, 0.08f, 12, panel);
        DrawText("LEAGUE STANDINGS", MAIN_X + 36, MAIN_Y + 28, FONT_TITLE, textMain);

        // Team list header
        DrawText("Team", CARD_X + 78, MAIN_Y + 106, FONT_SECTION, accent);
        DrawText("Points", CARD_X + 528, MAIN_Y + 106, FONT_SECTION, accent);
        DrawText("Goals", CARD_X + 644, MAIN_Y + 106, FONT_SECTION, accent);
        DrawText("Tier", CARD_X + 760, MAIN_Y + 106, FONT_SECTION, accent);

        // Team list as cards or empty state
        if (teams.empty()) {
            DrawText("No Teams Found", 600 - MeasureText("No Teams Found", 48)/2, 400, 48, textMuted);
        } else {
            int leaderPoints = teams[0].points > 0 ? teams[0].points : 1;
            for (const auto& t : teams) {
                if (t.points > leaderPoints) leaderPoints = t.points;
            }
            int maxTeamsShown = 5;
            int shownTeams = std::min((int)teams.size(), maxTeamsShown);
            for (int i = 0; i < (int)teams.size(); i++) {
                if (i >= shownTeams) break;
                int y = MAIN_Y + 156 + (i * 88);
                // Card shadow
                DrawRectangleRounded(Rectangle{(float)CARD_X + 3, (float)y + 7, (float)CARD_W - 6, 64}, 0.22f, 8, Fade(BLACK, 0.08f));
                // Card gradient (simulate with two rectangles)
                DrawRectangleRounded(Rectangle{(float)CARD_X, (float)y, (float)CARD_W, 64}, 0.22f, 8, cardTop);
                DrawRectangleRounded(Rectangle{(float)CARD_X, (float)y + 32, (float)CARD_W, 32}, 0.22f, 8, cardBottom);
                // Avatar (circle with initials)
                DrawCircle(CARD_X + 30, y + 32, 24, accentSoft);
                char initials[3] = {0};
                initials[0] = teams[i].name[0];
                for (int j = 1, k = 1; teams[i].name[j] != '\0' && k < 2; j++) {
                    if (teams[i].name[j-1] == ' ' && teams[i].name[j] != ' ') {
                        initials[k++] = teams[i].name[j];
                    }
                }
                DrawText(initials, CARD_X + 19, y + 19, 26, textMain);
                // Team name (truncate if too long)
                int nameX = CARD_X + 76, nameY = y + 14;
                DrawEllipsizedText(teams[i].name, Rectangle{(float)nameX, (float)nameY, 430, 32}, FONT_BODY, textMain, false);
                float progress = (float)teams[i].points / (float)leaderPoints;
                if (progress < 0.0f) progress = 0.0f;
                if (progress > 1.0f) progress = 1.0f;
                DrawRectangleRounded(Rectangle{(float)nameX, (float)y + 48, 430, 8}, 0.5f, 6, GetColor(0xE2E8F0FF));
                DrawRectangleRounded(Rectangle{(float)nameX, (float)y + 48, 430.0f * progress, 8}, 0.5f, 6, accent);
                // Points
                DrawEllipsizedText(TextFormat("%d", teams[i].points), Rectangle{(float)CARD_X + 524, (float)y + 14, 86, 36}, FONT_BODY, warning, true);
                // Goals (fit in box)
                char goalsBuf[16];
                snprintf(goalsBuf, 15, "%d", teams[i].goalsScored);
                DrawEllipsizedText(goalsBuf, Rectangle{(float)CARD_X + 640, (float)y + 14, 86, 36}, FONT_BODY, success, true);
                DrawEllipsizedText(GetScoreTier(teams[i]), Rectangle{(float)CARD_X + 750, (float)y + 14, 100, 36}, FONT_SMALL, textMuted, true);
                Rectangle minusPointBtn = Rectangle{(float)CARD_X + 500, (float)y + 22, 18, 18};
                Rectangle plusPointBtn = Rectangle{(float)CARD_X + 614, (float)y + 22, 18, 18};
                DrawRectangleRounded(minusPointBtn, 0.4f, 4, GetColor(0xCBD5E1FF));
                DrawRectangleRounded(plusPointBtn, 0.4f, 4, GetColor(0xCBD5E1FF));
                DrawText("-", (int)minusPointBtn.x + 6, (int)minusPointBtn.y - 1, 20, textMain);
                DrawText("+", (int)plusPointBtn.x + 4, (int)plusPointBtn.y - 1, 20, textMain);
                if (!showAddModal && !showEditModal && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mouse, minusPointBtn) && teams[i].points > 0) teams[i].points--;
                    if (CheckCollisionPointRec(mouse, plusPointBtn)) teams[i].points++;
                }
                // Edit button (pencil icon)
                Rectangle editBtn = Rectangle{(float)CARD_X + 852, (float)y + 14, 36, 36};
                DrawRectangleRounded(editBtn, 0.4f, 8, GetColor(0xCBD5E1FF));
                // Pencil icon
                DrawRectangle(editBtn.x+10, editBtn.y+20, 16, 4, textMain);
                DrawRectangle(editBtn.x+18, editBtn.y+10, 4, 16, accent);
                if (!showAddModal && !showEditModal && CheckCollisionPointRec(mouse, editBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    showEditModal = true;
                    editTeamIdx = i;
                    strncpy_s(editNameBuf, sizeof(editNameBuf), teams[i].name, 49); editNameBuf[49] = '\0';
                    editGoals = teams[i].goalsScored;
                    editPoints = teams[i].points;
                    editNameActive = true;
                    editGoalsActive = false;
                    editPointsActive = false;
                }
            }
            if ((int)teams.size() > shownTeams) {
                DrawText(TextFormat("+ %d more teams not shown", (int)teams.size() - shownTeams), CARD_X + 8, MAIN_Y + MAIN_H - 28, FONT_BODY, textMuted);
            }
        }

        // Footer
        DrawRectangleRounded(Rectangle{(float)FOOTER_X, (float)FOOTER_Y, (float)FOOTER_W, (float)FOOTER_H}, 0.08f, 8, GetColor(0xE5E7EBFF));
        // Dynamic total goals from teams
        int totalGoals = 0;
        for (const auto& t : teams) totalGoals += t.goalsScored;
        DrawText(TextFormat("Total Goals: %d", totalGoals), FOOTER_X + 26, FOOTER_Y + 20, FONT_SECTION, textMain);
        DrawText("ESC: Exit | F1:Sort | F2:Delete Last | F3:Clear All", FOOTER_X + 340, FOOTER_Y + 24, FONT_META, textMuted);
        DrawText("+/- buttons change points directly", FOOTER_X + 340, FOOTER_Y + 46, FONT_META, textMuted);
        DrawText("© 2024 BinaryBeasts", FOOTER_X + FOOTER_W - 150, FOOTER_Y + 78, FONT_META, textMuted);

        // Score insights panel
        DrawRectangleRounded(Rectangle{(float)FOOTER_X + 12, (float)FOOTER_Y + 76, (float)FOOTER_W - 24, 24}, 0.25f, 8, GetColor(0xE0F2FEFF));
        if (!teams.empty()) {
            const Team* bestTeam = &teams[0];
            for (const auto& t : teams) {
                if (t.points > bestTeam->points || (t.points == bestTeam->points && t.goalsScored > bestTeam->goalsScored)) {
                    bestTeam = &t;
                }
            }
            float avgPoints = 0.0f;
            for (const auto& t : teams) avgPoints += (float)t.points;
            avgPoints /= (float)teams.size();
            DrawText(
                TextFormat("Leader: %s (%d pts) | Avg points: %.1f | Leader tier: %s",
                    bestTeam->name, bestTeam->points, avgPoints, GetScoreTier(*bestTeam)),
                FOOTER_X + 22, FOOTER_Y + 79, FONT_META, GetColor(0x0369A1FF));
        } else {
            DrawText("Add a team to unlock score insights.", FOOTER_X + 22, FOOTER_Y + 79, FONT_META, GetColor(0x0369A1FF));
        }

        // Add Team Modal
        if (showAddModal) {
            DrawRectangle(0, 0, 1200, 800, Fade(BLACK, 0.38f));
            // Modal shadow
            DrawRectangleRounded(Rectangle{384, 234, 512, 292}, 0.22f, 12, Fade(BLACK, 0.16f));
            DrawRectangleRounded(Rectangle{380, 230, 520, 300}, 0.22f, 12, panel);
            DrawText("Add New Team", 480, 265, FONT_TITLE, textMain);
            // Input box
            DrawRectangleRounded(Rectangle{440, 340, 400, 70}, 0.22f, 12, inputActive ? GetColor(0xE0F2FEFF) : GetColor(0xE5E7EBFF));
            DrawEllipsizedText(inputBuf[0] ? inputBuf : "Team name...", Rectangle{452, 344, 376, 62}, FONT_BODY, textMain, false);
            DrawText("Press ENTER to add, ESC to cancel", 440, 425, FONT_SMALL, textMuted);
            if (CheckCollisionPointRec(mouse, Rectangle{440, 340, 400, 70}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                inputActive = true;
            }
        }

        // Edit Team Modal
        if (showEditModal && editTeamIdx >= 0 && editTeamIdx < (int)teams.size()) {
            DrawRectangle(0, 0, 1200, 800, Fade(BLACK, 0.38f));
            // Modal shadow and background (match add modal)
            DrawRectangleRounded(Rectangle{384, 234, 512, 292}, 0.22f, 12, Fade(BLACK, 0.16f));
            DrawRectangleRounded(Rectangle{380, 230, 520, 300}, 0.22f, 12, panel);
            DrawText("Edit Team", 520, 265, FONT_TITLE, textMain);
            // Name input
            DrawText("Name:", 420, 340, FONT_BODY, textMain);
            DrawRectangleRounded(Rectangle{520, 335, 340, 60}, 0.22f, 12, editNameActive ? GetColor(0xE0F2FEFF) : GetColor(0xE5E7EBFF));
            DrawEllipsizedText(editNameBuf[0] ? editNameBuf : "Team name...", Rectangle{532, 337, 320, 56}, FONT_BODY, textMain, false);
            // Goals input
            DrawText("Goals:", 420, 420, FONT_BODY, textMain);
            DrawRectangleRounded(Rectangle{520, 415, 140, 60}, 0.22f, 12, editGoalsActive ? GetColor(0xE0F2FEFF) : GetColor(0xE5E7EBFF));
            char goalsShow[16];
            snprintf(goalsShow, 15, "%d", editGoals);
            DrawEllipsizedText(goalsShow, Rectangle{532, 417, 116, 56}, FONT_BODY, textMain, true);
            DrawText("Points:", 680, 420, FONT_BODY, textMain);
            DrawRectangleRounded(Rectangle{790, 415, 70, 60}, 0.22f, 12, editPointsActive ? GetColor(0xE0F2FEFF) : GetColor(0xE5E7EBFF));
            char pointsShow[16];
            snprintf(pointsShow, 15, "%d", editPoints);
            DrawEllipsizedText(pointsShow, Rectangle{798, 417, 54, 56}, FONT_SMALL, textMain, true);
            // Save button
            Rectangle saveBtn = Rectangle{700, 500, 140, 48};
            DrawRectangleRounded(saveBtn, 0.22f, 12, accent);
            DrawText("Save", 740, 510, 32, WHITE);
            // Input handling
            if (CheckCollisionPointRec(mouse, Rectangle{520, 335, 340, 60}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                editNameActive = true; editGoalsActive = false;
            }
            if (CheckCollisionPointRec(mouse, Rectangle{520, 415, 140, 60}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                editGoalsActive = true; editNameActive = false; editPointsActive = false;
            }
            if (CheckCollisionPointRec(mouse, Rectangle{790, 415, 70, 60}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                editPointsActive = true; editNameActive = false; editGoalsActive = false;
            }
            if (CheckCollisionPointRec(mouse, saveBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                strncpy_s(teams[editTeamIdx].name, sizeof(teams[editTeamIdx].name), editNameBuf, 49); teams[editTeamIdx].name[49] = '\0';
                teams[editTeamIdx].goalsScored = editGoals;
                teams[editTeamIdx].points = editPoints;
                showEditModal = false; editTeamIdx = -1;
            }
            // Keyboard input
            if (editNameActive) {
                int key = GetCharPressed();
                int len = (int)strlen(editNameBuf);
                while (key > 0) {
                    if (len < 49 && key >= 32 && key <= 125) {
                        editNameBuf[len] = (char)key;
                        editNameBuf[len+1] = '\0';
                        len++;
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && len > 0) editNameBuf[--len] = '\0';
            }
            if (editGoalsActive) {
                if (IsKeyPressed(KEY_UP)) editGoals++;
                if (IsKeyPressed(KEY_DOWN) && editGoals > 0) editGoals--;
                // Direct number input
                int key = GetCharPressed();
                if (key >= '0' && key <= '9') {
                    editGoals = editGoals * 10 + (key - '0');
                    if (editGoals > 9999) editGoals = 9999;
                }
                if (IsKeyPressed(KEY_BACKSPACE)) editGoals /= 10;
            }
            if (editPointsActive) {
                if (IsKeyPressed(KEY_UP)) editPoints++;
                if (IsKeyPressed(KEY_DOWN) && editPoints > 0) editPoints--;
                int key = GetCharPressed();
                if (key >= '0' && key <= '9') {
                    editPoints = editPoints * 10 + (key - '0');
                    if (editPoints > 999) editPoints = 999;
                }
                if (IsKeyPressed(KEY_BACKSPACE)) editPoints /= 10;
            }
            if (IsKeyPressed(KEY_ESCAPE)) { showEditModal = false; editTeamIdx = -1; }
        }

        EndDrawing();
    }
    save_data(teams);
    CloseWindow();
}