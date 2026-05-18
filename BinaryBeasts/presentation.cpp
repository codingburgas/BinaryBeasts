#if __has_include("raylib.h")
#if __has_include("raylib.h")
#  include "raylib.h"
#else
#  include "raylib_stubs.h"
#endif

#include "football_menu.h"
#include "logic.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cstring>

#ifndef __RAYLIB_H__
inline int GetFrameCount() { static int f = 0; return ++f; }
#endif

using namespace std;

static const Rectangle SEARCH_BOX{48, 598, 210, 48};
static const int SEARCH_FONT = 20;

static int searchTeamIndexLogic(const vector<Team>& teams, const char* query, int sortMode) {
    if (query == nullptr || query[0] == '\0') return -1;
    int idx = findTeamByNamePartialLogic(teams, query);
    if (idx < 0) {
        if (sortMode == 2) idx = findTeamByNameBinaryLogic(teams, query);
        else idx = findTeamByNameLinearLogic(teams, query);
    }
    return idx;
}

static void runLiveSearch(const vector<Team>& teams, const char* query, int sortMode, int* foundTeamIndex) {
    if (foundTeamIndex == nullptr) return;
    *foundTeamIndex = (query != nullptr && query[0] != '\0')
        ? searchTeamIndexLogic(teams, query, sortMode)
        : -1;
}

static void updateSearchField(
    char* buf, int bufSize, bool* active, int* foundTeamIndex,
    const vector<Team>& teams, int sortMode, Vector2 mouse)
{
    if (active == nullptr || buf == nullptr || foundTeamIndex == nullptr) return;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *active = CheckCollisionPointRec(mouse, SEARCH_BOX);
    }
    if (IsKeyPressed(KEY_SLASH)) {
        *active = true;
    }

    int len = (int)strlen(buf);

    if (*active && IsKeyPressed(KEY_ESCAPE)) {
        buf[0] = '\0';
        *active = false;
        *foundTeamIndex = -1;
        return;
    }
    if (*active && IsKeyPressed(KEY_DELETE)) {
        buf[0] = '\0';
        len = 0;
        *foundTeamIndex = -1;
    }
    if (*active && (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))) {
        if (len > 0) {
            buf[len - 1] = '\0';
            len--;
        }
    }

    int key = GetCharPressed();
    while (key > 0) {
        if (key == '/') {
            *active = true;
        } else if (key >= 32 && key <= 125) {
            if (!*active) *active = true;
            if (len < bufSize - 1) {
                buf[len++] = (char)key;
                buf[len] = '\0';
            }
        }
        key = GetCharPressed();
    }

    runLiveSearch(teams, buf, sortMode, foundTeamIndex);
}

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

void runApp() {
    InitWindow(1200, 800, "Pitch & Score");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    srand((unsigned int)time(NULL));

    AppScreen screen = AppScreen::MainMenu;
    bool shouldQuit = false;

    vector<Team> teams = loadTeamsLogic();

    
    if (teams.empty()) {
        addTeamLogic(teams, "Real Madrid");
        addTeamLogic(teams, "FC Barcelona");
        addTeamLogic(teams, "Manchester City");
    }

    char inputBuf[50] = "\0";
    int letterCount = 0;
    bool showAddModal = false;
    bool inputActive = false;
    
    bool showEditModal = false;
    int editTeamIdx = -1;
    char editNameBuf[50] = "\0";
    int editGoals = 0;
    int editPoints = 0;
    bool editNameActive = false;
    bool editGoalsActive = false;
    bool editPointsActive = false;

    
    int sortMode = 0;
    const char* sortLabels[3] = {"Sort: Points", "Sort: Goals", "Sort: Name"};
    char searchBuf[50] = "\0";
    bool searchInputActive = false;
    int foundTeamIndex = -1;
    while (!WindowShouldClose() && !shouldQuit) {
        if (screen == AppScreen::LeagueStandings && showAddModal) {
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
                int valid = 1;
                for (int i = 0; inputBuf[i]; ++i) {
                    if (inputBuf[i] < 32) valid = 0;
                }
                if (valid) {
                    addTeamLogic(teams, inputBuf);
                    persistTeamsLogic(teams);
                }
                inputBuf[0] = '\0'; letterCount = 0;
                showAddModal = false;
                inputActive = false;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                showAddModal = false;
                inputActive = false;
            }
        } else if (screen == AppScreen::LeagueStandings) {
            if (IsKeyPressed(KEY_F1) && teams.size() > 1) {
                sortMode = (sortMode + 1) % 3;
                sortTeamsByModeLogic(teams, sortMode);
                runLiveSearch(teams, searchBuf, sortMode, &foundTeamIndex);
            }
            if (IsKeyPressed(KEY_F2)) { deleteLastTeamLogic(teams); persistTeamsLogic(teams); foundTeamIndex = -1; }
            if (IsKeyPressed(KEY_F3)) { clearAllTeamsLogic(teams); persistTeamsLogic(teams); foundTeamIndex = -1; searchBuf[0] = '\0'; }
            if (!showEditModal && !showAddModal) {
                updateSearchField(searchBuf, (int)sizeof(searchBuf), &searchInputActive, &foundTeamIndex, teams, sortMode, GetMousePosition());
            }
        }

        BeginDrawing();
        Vector2 mouse = GetMousePosition();

        switch (screen) {
        case AppScreen::MainMenu: {
            AppScreen next = UpdateAndDrawMainMenu(mouse, 1200, 800);
            if (next == AppScreen::Quit || IsKeyPressed(KEY_ESCAPE))
                shouldQuit = true;
            else if (next != AppScreen::MainMenu)
                screen = next;
            EndDrawing();
            continue;
        }
        case AppScreen::LiveScores: {
            bool gb = false;
            DrawLiveScoresScreen(teams, mouse, 1200, 800, &gb);
            if (gb) screen = AppScreen::MainMenu;
            EndDrawing();
            continue;
        }
        case AppScreen::MatchSchedule: {
            bool gb = false;
            DrawMatchScheduleScreen(teams, mouse, 1200, 800, &gb);
            if (gb) screen = AppScreen::MainMenu;
            EndDrawing();
            continue;
        }
        case AppScreen::SeasonStats: {
            bool gb = false;
            DrawSeasonStatsScreen(teams, mouse, 1200, 800, &gb);
            if (gb) screen = AppScreen::MainMenu;
            EndDrawing();
            continue;
        }
        case AppScreen::About: {
            bool gb = false;
            DrawAboutScreen(mouse, 1200, 800, &gb);
            if (gb) screen = AppScreen::MainMenu;
            EndDrawing();
            continue;
        }
        case AppScreen::Quit:
            shouldQuit = true;
            EndDrawing();
            continue;
        case AppScreen::LeagueStandings:
        default:
            break;
        }
        Color pitch = GetColor(0x166534FF);
        Color pitchDark = GetColor(0x14532DFF);
        Color bg = pitch;
        Color sidebar = pitchDark;
        Color panel = GetColor(0xF0FDF4FF);
        Color cardTop = GetColor(0xFFFFFFFF);
        Color cardBottom = GetColor(0xECFDF5FF);
        Color accent = GetColor(0xFACC15FF);
        Color accentSoft = GetColor(0x22C55EFF);
        Color textMain = GetColor(0x052E16FF);
        Color textMuted = GetColor(0x365314FF);
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
        for (int y = 0; y < 800; y += 80) {
            DrawRectangle(0, y, 1200, 40, (y / 80) % 2 == 0 ? Fade(pitch, 1.0f) : Fade(pitchDark, 0.35f));
        }

        DrawRectangle(0, 0, SIDEBAR_W, 800, sidebar);
        DrawText("PITCH\n&SCORE", 58, 50, 30, accent);

        int btnY = 228;
        int btnH = 64;
        int btnPad = 24;
        Color btnColor = GetColor(0x1E3A2FFF);
        Color btnHover = GetColor(0x15803DFF);

        Rectangle homeBtn{50, 132, 160, 40};
        bool homeHover = CheckCollisionPointRec(mouse, homeBtn);
        DrawRectangleRounded(homeBtn, 0.2f, 8, homeHover ? btnHover : btnColor);
        DrawText("Main menu", 70, 142, 18, WHITE);

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
            if (btnHovers[b]) DrawRectangle(36, (int)btnRects[b].y, 8, btnH, accent);
            DrawRectangleRounded(btnRects[b], 0.22f, 8, btnHovers[b] ? btnHover : btnColor);

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
            if (homeHover) screen = AppScreen::MainMenu;
            if (btnHovers[0]) { showAddModal = true; inputActive = true; }
            if (btnHovers[1] && teams.size() > 1) {
                sortMode = (sortMode + 1) % 3;
                sortTeamsByModeLogic(teams, sortMode);
                runLiveSearch(teams, searchBuf, sortMode, &foundTeamIndex);
            }
            if (btnHovers[2]) { deleteLastTeamLogic(teams); persistTeamsLogic(teams); foundTeamIndex = -1; }
            if (btnHovers[3]) { clearAllTeamsLogic(teams); persistTeamsLogic(teams); foundTeamIndex = -1; }
        }

        bool searchHover = CheckCollisionPointRec(mouse, SEARCH_BOX);
        DrawRectangleRounded(SEARCH_BOX, 0.2f, 8, searchInputActive ? GetColor(0xE0F2FEFF) : btnColor);
        DrawRectangleRoundedLinesEx(SEARCH_BOX, 0.2f, 8, 2.0f, searchInputActive || searchHover ? accent : Fade(WHITE, 0.25f));

        Rectangle searchTextBox{SEARCH_BOX.x + 10, SEARCH_BOX.y + 6, SEARCH_BOX.width - 20, SEARCH_BOX.height - 12};
        const char* searchPlaceholder = "Search team...";
        Color searchTextCol = searchInputActive ? textMain : WHITE;
        if (searchBuf[0]) {
            DrawEllipsizedText(searchBuf, searchTextBox, SEARCH_FONT, searchTextCol, false);
            if (searchInputActive && (GetFrameCount() / 30) % 2 == 0) {
                int cursorX = (int)searchTextBox.x + MeasureText(searchBuf, SEARCH_FONT) + 2;
                int cursorY = (int)searchTextBox.y + ((int)searchTextBox.height - SEARCH_FONT) / 2;
                DrawRectangle(cursorX, cursorY, 2, SEARCH_FONT, searchTextCol);
            }
        } else {
            DrawEllipsizedText(searchPlaceholder, searchTextBox, SEARCH_FONT, Fade(WHITE, 0.55f), false);
        }

        if (foundTeamIndex >= 0 && foundTeamIndex < (int)teams.size()) {
            DrawEllipsizedText(
                TextFormat("Found: #%d %s", foundTeamIndex + 1, teams[foundTeamIndex].name),
                Rectangle{SEARCH_BOX.x, SEARCH_BOX.y + SEARCH_BOX.height + 6, SEARCH_BOX.width, 24}, FONT_META, accent, false);
        } else if (searchInputActive && searchBuf[0] != '\0') {
            DrawEllipsizedText("No match", Rectangle{SEARCH_BOX.x, SEARCH_BOX.y + SEARCH_BOX.height + 6, SEARCH_BOX.width, 24}, FONT_META, Fade(WHITE, 0.8f), false);
        }

        // Main content area
        DrawRectangleRounded(Rectangle{(float)MAIN_X, (float)MAIN_Y, (float)MAIN_W, (float)MAIN_H}, 0.08f, 12, panel);
        DrawText("LEAGUE STANDINGS", MAIN_X + 36, MAIN_Y + 28, FONT_TITLE, textMain);

        // Team list header
        DrawText("Team", CARD_X + 78, MAIN_Y + 106, FONT_SECTION, accent);
        DrawText("Points", CARD_X + 528, MAIN_Y + 106, FONT_SECTION, accent);
        DrawText("Goals", CARD_X + 644, MAIN_Y + 106, FONT_SECTION, accent);
        DrawText("Tier", CARD_X + 760, MAIN_Y + 106, FONT_SECTION, accent);

        
        if (teams.empty()) {
            DrawText("No Teams Found", 600 - MeasureText("No Teams Found", 48)/2, 400, 48, textMuted);
        } else {
            int leaderPoints = teams[0].points > 0 ? teams[0].points : 1;
            for (const auto& t : teams) {
                if (t.points > leaderPoints) leaderPoints = t.points;
            }
            int maxTeamsShown = 5;
            int listStart = 0;
            if (foundTeamIndex >= maxTeamsShown) {
                listStart = foundTeamIndex - maxTeamsShown + 1;
            }
            int listEnd = min(listStart + maxTeamsShown, (int)teams.size());
            for (int i = listStart; i < listEnd; i++) {
                int y = MAIN_Y + 156 + ((i - listStart) * 88);
                bool isFound = (foundTeamIndex == i);
                
                DrawRectangleRounded(Rectangle{(float)CARD_X + 3, (float)y + 7, (float)CARD_W - 6, 64}, 0.22f, 8, Fade(BLACK, 0.08f));
                
                DrawRectangleRounded(Rectangle{(float)CARD_X, (float)y, (float)CARD_W, 64}, 0.22f, 8, cardTop);
                if (isFound) {
                    DrawRectangleRoundedLinesEx(Rectangle{(float)CARD_X, (float)y, (float)CARD_W, 64}, 0.22f, 8, 3.0f, accent);
                }
                DrawRectangleRounded(Rectangle{(float)CARD_X, (float)y + 32, (float)CARD_W, 32}, 0.22f, 8, cardBottom);
                
                DrawCircle(CARD_X + 30, y + 32, 24, accentSoft);
                char initials[3] = {0};
                initials[0] = teams[i].name[0];
                for (int j = 1, k = 1; teams[i].name[j] != '\0' && k < 2; j++) {
                    if (teams[i].name[j-1] == ' ' && teams[i].name[j] != ' ') {
                        initials[k++] = teams[i].name[j];
                    }
                }
                DrawText(initials, CARD_X + 19, y + 19, 26, textMain);
                
                int nameX = CARD_X + 76, nameY = y + 14;
                DrawEllipsizedText(teams[i].name, Rectangle{(float)nameX, (float)nameY, 430, 32}, FONT_BODY, textMain, false);
                float progress = (float)teams[i].points / (float)leaderPoints;
                if (progress < 0.0f) progress = 0.0f;
                if (progress > 1.0f) progress = 1.0f;
                DrawRectangleRounded(Rectangle{(float)nameX, (float)y + 48, 430, 8}, 0.5f, 6, GetColor(0xE2E8F0FF));
                DrawRectangleRounded(Rectangle{(float)nameX, (float)y + 48, 430.0f * progress, 8}, 0.5f, 6, accent);
                
                DrawEllipsizedText(TextFormat("%d", teams[i].points), Rectangle{(float)CARD_X + 524, (float)y + 14, 86, 36}, FONT_BODY, warning, true);
                
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
                    if (CheckCollisionPointRec(mouse, minusPointBtn) && teams[i].points > 0) { teams[i].points--; persistTeamsLogic(teams); }
                    if (CheckCollisionPointRec(mouse, plusPointBtn)) { teams[i].points++; persistTeamsLogic(teams); }
                }
                
                Rectangle editBtn = Rectangle{(float)CARD_X + 852, (float)y + 14, 36, 36};
                DrawRectangleRounded(editBtn, 0.4f, 8, GetColor(0xCBD5E1FF));
                
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
            if ((int)teams.size() > maxTeamsShown) {
                DrawText(
                    TextFormat("Showing %d-%d of %d teams", listStart + 1, listEnd, (int)teams.size()),
                    CARD_X + 8, MAIN_Y + MAIN_H - 28, FONT_BODY, textMuted);
            }
        }

        
        DrawRectangleRounded(Rectangle{(float)FOOTER_X, (float)FOOTER_Y, (float)FOOTER_W, (float)FOOTER_H}, 0.08f, 8, GetColor(0xE5E7EBFF));
        
        int totalGoals = calculateTotalGoalsFromTeamsRecursive(teams, (int)teams.size());
        DrawText(TextFormat("Total Goals: %d", totalGoals), FOOTER_X + 26, FOOTER_Y + 20, FONT_SECTION, textMain);
        DrawEllipsizedText(
            "Main menu: sidebar | F1: Sort | F2: Delete last | F3: Clear all",
            Rectangle{(float)FOOTER_X + 280, (float)FOOTER_Y + 22, (float)FOOTER_W - 296, 22},
            FONT_META, textMuted, false);
        DrawEllipsizedText(
            "Search: type to filter | / focus | ESC clear | partial match",
            Rectangle{(float)FOOTER_X + 280, (float)FOOTER_Y + 44, (float)FOOTER_W - 296, 22},
            FONT_META, textMuted, false);
        DrawEllipsizedText(
            "(c) 2024 BinaryBeasts",
            Rectangle{(float)FOOTER_X + FOOTER_W - 168, (float)FOOTER_Y + 20, 156, 22},
            FONT_META, textMuted, false);

        
        const float insightsBarY = (float)FOOTER_Y + 74;
        const float insightsBarH = 28.0f;
        DrawRectangleRounded(
            Rectangle{(float)FOOTER_X + 12, insightsBarY, (float)FOOTER_W - 24, insightsBarH},
            0.25f, 8, GetColor(0xE0F2FEFF));
        Rectangle insightsTextBox{(float)FOOTER_X + 22, insightsBarY + 2, (float)FOOTER_W - 44, insightsBarH - 4};
        Color insightsText = GetColor(0x0369A1FF);
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
            DrawEllipsizedText(
                TextFormat("Leader: %s (%d pts) | Avg points: %.1f | Leader tier: %s",
                    bestTeam->name, bestTeam->points, avgPoints, GetScoreTier(*bestTeam)),
                insightsTextBox, FONT_META, insightsText, false);
        } else {
            DrawEllipsizedText("Add a team to unlock score insights.", insightsTextBox, FONT_META, insightsText, false);
        }

        
        if (showAddModal) {
            DrawRectangle(0, 0, 1200, 800, Fade(BLACK, 0.38f));
            
            DrawRectangleRounded(Rectangle{384, 234, 512, 292}, 0.22f, 12, Fade(BLACK, 0.16f));
            DrawRectangleRounded(Rectangle{380, 230, 520, 300}, 0.22f, 12, panel);
            DrawText("Add New Team", 480, 265, FONT_TITLE, textMain);
            
            DrawRectangleRounded(Rectangle{440, 340, 400, 70}, 0.22f, 12, inputActive ? GetColor(0xE0F2FEFF) : GetColor(0xE5E7EBFF));
            DrawEllipsizedText(inputBuf[0] ? inputBuf : "Team name...", Rectangle{452, 344, 376, 62}, FONT_BODY, textMain, false);
            DrawText("Press ENTER to add, ESC to cancel", 440, 425, FONT_SMALL, textMuted);
            if (CheckCollisionPointRec(mouse, Rectangle{440, 340, 400, 70}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                inputActive = true;
            }
        }

        
        if (showEditModal && editTeamIdx >= 0 && editTeamIdx < (int)teams.size()) {
            const float modalX = 380.0f;
            const float modalY = 220.0f;
            const float modalW = 520.0f;
            const float modalH = 340.0f;
            const int fieldH = 60;
            const int labelFieldPadY = (fieldH - FONT_BODY) / 2;

            DrawRectangle(0, 0, 1200, 800, Fade(BLACK, 0.38f));
            DrawRectangleRounded(Rectangle{modalX + 4, modalY + 4, modalW, modalH}, 0.22f, 12, Fade(BLACK, 0.16f));
            DrawRectangleRounded(Rectangle{modalX, modalY, modalW, modalH}, 0.22f, 12, panel);
            DrawText("Edit Team", 520, (int)modalY + 42, FONT_TITLE, textMain);

            const int nameFieldY = (int)modalY + 118;
            DrawText("Name:", 420, nameFieldY + labelFieldPadY, FONT_BODY, textMain);
            DrawRectangleRounded(Rectangle{520, (float)nameFieldY, 340, (float)fieldH}, 0.22f, 12, editNameActive ? GetColor(0xE0F2FEFF) : GetColor(0xE5E7EBFF));
            DrawEllipsizedText(editNameBuf[0] ? editNameBuf : "Team name...", Rectangle{532, (float)nameFieldY + 2, 320, (float)fieldH - 4}, FONT_BODY, textMain, false);

            const int statFieldY = (int)modalY + 198;
            DrawText("Goals:", 420, statFieldY + labelFieldPadY, FONT_BODY, textMain);
            DrawRectangleRounded(Rectangle{520, (float)statFieldY, 140, (float)fieldH}, 0.22f, 12, editGoalsActive ? GetColor(0xE0F2FEFF) : GetColor(0xE5E7EBFF));
            char goalsShow[16];
            snprintf(goalsShow, 15, "%d", editGoals);
            DrawEllipsizedText(goalsShow, Rectangle{532, (float)statFieldY + 2, 116, (float)fieldH - 4}, FONT_BODY, textMain, true);
            DrawText("Points:", 680, statFieldY + labelFieldPadY, FONT_BODY, textMain);
            DrawRectangleRounded(Rectangle{790, (float)statFieldY, 100, (float)fieldH}, 0.22f, 12, editPointsActive ? GetColor(0xE0F2FEFF) : GetColor(0xE5E7EBFF));
            char pointsShow[16];
            snprintf(pointsShow, 15, "%d", editPoints);
            DrawEllipsizedText(pointsShow, Rectangle{802, (float)statFieldY + 2, 76, (float)fieldH - 4}, FONT_BODY, textMain, true);

            Rectangle saveBtn = Rectangle{modalX + modalW - 180, modalY + modalH - 68, 140, 48};
            DrawRectangleRounded(saveBtn, 0.22f, 12, accent);
            DrawText("Save", (int)saveBtn.x + 36, (int)saveBtn.y + 12, FONT_SECTION, WHITE);
            
            if (CheckCollisionPointRec(mouse, Rectangle{520, (float)nameFieldY, 340, (float)fieldH}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                editNameActive = true; editGoalsActive = false;
            }
            if (CheckCollisionPointRec(mouse, Rectangle{520, (float)statFieldY, 140, (float)fieldH}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                editGoalsActive = true; editNameActive = false; editPointsActive = false;
            }
            if (CheckCollisionPointRec(mouse, Rectangle{790, (float)statFieldY, 100, (float)fieldH}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                editPointsActive = true; editNameActive = false; editGoalsActive = false;
            }
            if (CheckCollisionPointRec(mouse, saveBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                strncpy_s(teams[editTeamIdx].name, sizeof(teams[editTeamIdx].name), editNameBuf, 49); teams[editTeamIdx].name[49] = '\0';
                teams[editTeamIdx].goalsScored = editGoals;
                teams[editTeamIdx].points = editPoints;
                persistTeamsLogic(teams);
                showEditModal = false; editTeamIdx = -1;
            }
            
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
    persistTeamsLogic(teams);
    CloseWindow();
}

#endif
