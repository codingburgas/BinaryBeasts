#include "football_menu.h"
#include "logic.h"

static void DrawFittedCenter(const char* text, Rectangle box, int startFont, int minFont, Color color) {
    int fontSize = startFont;
    while (MeasureText(text, fontSize) > (int)box.width - 16 && fontSize > minFont) fontSize--;
    int tw = MeasureText(text, fontSize);
    int x = (int)box.x + ((int)box.width - tw) / 2;
    int y = (int)box.y + ((int)box.height - fontSize) / 2;
    DrawText(text, x, y, fontSize, color);
}

static bool MenuButton(Rectangle r, Vector2 mouse, const char* label, Color base, Color hover, Color textCol) {
    bool over = CheckCollisionPointRec(mouse, r);
    DrawRectangleRounded(r, 0.2f, 10, over ? hover : base);
    DrawRectangleRoundedLinesEx(r, 0.2f, 10, 2.0f, Fade(WHITE, 0.35f));
    DrawFittedCenter(label, Rectangle{r.x + 8, r.y + 4, r.width - 16, r.height - 8}, 22, 16, textCol);
    return over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void DrawSubScreenChrome(const char* title, Vector2 mouse, int screenW, int screenH, bool* goBack) {
    (void)screenH;
    const int barH = 56;
    Color bar = GetColor(0x14532DFF);
    Color accent = GetColor(0xFACC15FF);
    DrawRectangle(0, 0, screenW, barH, bar);
    DrawRectangle(0, barH - 3, screenW, 3, accent);

    Rectangle backR{12, 10, 100, 36};
    if (MenuButton(backR, mouse, "< Back", GetColor(0x166534FF), GetColor(0x15803DFF), WHITE)) {
        if (goBack) *goBack = true;
    }

    int titleW = MeasureText(title, 28);
    DrawText(title, (screenW - titleW) / 2, 14, 28, WHITE);

    if (IsKeyPressed(KEY_ESCAPE) && goBack) *goBack = true;
}

AppScreen UpdateAndDrawMainMenu(Vector2 mouse, int screenW, int screenH) {
    Color pitch = GetColor(0x166534FF);
    Color pitchDark = GetColor(0x14532DFF);
    ClearBackground(pitch);
    for (int y = 0; y < screenH; y += 80) {
        DrawRectangle(0, y, screenW, 40, (y / 80) % 2 == 0 ? Fade(pitch, 1.0f) : Fade(pitchDark, 0.35f));
    }
    int midX = screenW / 2;
    DrawLine(midX, 0, midX, screenH, Fade(WHITE, 0.2f));
    DrawCircle(midX, screenH / 2, 72, Fade(WHITE, 0.15f));
    DrawCircleLines(midX, screenH / 2, 72, Fade(WHITE, 0.35f));

    Color gold = GetColor(0xFACC15FF);
    const char* brand = "PITCH & SCORE";
    int bw = MeasureText(brand, 44);
    DrawText(brand, (screenW - bw) / 2, 48, 44, gold);
    const char* tag = "Football league desk — standings, live, schedule, stats";
    int tw = MeasureText(tag, 20);
    DrawText(tag, (screenW - tw) / 2, 100, 20, Fade(WHITE, 0.85f));

    const int btnW = 340;
    const int btnH = 48;
    const int gap = 14;
    int startX = (screenW - btnW) / 2;
    int startY = 200;

    Color btn = GetColor(0x1E3A2FFF);
    Color btnHi = GetColor(0x22C55EFF);

    struct Item { const char* label; AppScreen screen; };
    Item items[] = {
        {"League table & points", AppScreen::LeagueStandings},
        {"Live scores (simulated)", AppScreen::LiveScores},
        {"Match schedule", AppScreen::MatchSchedule},
        {"Season stats", AppScreen::SeasonStats},
        {"About this app", AppScreen::About},
        {"Exit", AppScreen::Quit},
    };

    for (int i = 0; i < 6; i++) {
        Rectangle r{(float)startX, (float)(startY + i * (btnH + gap)), (float)btnW, (float)btnH};
        if (MenuButton(r, mouse, items[i].label, btn, btnHi, WHITE)) {
            return items[i].screen;
        }
    }

    DrawText("Click a tile to open — ESC exits from menu", startX, screenH - 48, 18, Fade(WHITE, 0.6f));
    return AppScreen::MainMenu;
}

static void SubScreenBackground(int screenW, int screenH) {
    Color bg = GetColor(0xECFDF5FF);
    ClearBackground(bg);
    DrawRectangle(0, 56, screenW, screenH - 56, bg);
}

void DrawLiveScoresScreen(const std::vector<Team>& teams, Vector2 mouse, int screenW, int screenH, bool* goBack) {
    bool back = false;
    SubScreenBackground(screenW, screenH);
    DrawSubScreenChrome("Live scores", mouse, screenW, screenH, &back);

    int y = 76;
    Color card = WHITE;
    Color textMain = GetColor(0x14532DFF);
    Color live = GetColor(0xDC2626FF);

    if (teams.size() < 2) {
        DrawText("Add at least two teams in League table to see fixtures.", 40, y + 20, 22, textMain);
    } else {
        int n = (int)teams.size();
        int row = 0;
        while (y < screenH - 100 && row < n && row < 6) {
            int a = row % n;
            int b = (row + 1) % n;
            int ha = teams[a].goalsScored % 5;
            int hb = teams[b].goalsScored % 5;
            Rectangle r{40, (float)y, (float)screenW - 80, 72};
            DrawRectangleRounded(r, 0.12f, 10, card);
            DrawRectangleRoundedLinesEx(r, 0.12f, 10, 2.0f, Fade(GetColor(0x22C55EFF), 0.5f));

            DrawText(teams[a].name, 56, y + 16, 22, textMain);
            int scoreW = MeasureText(TextFormat("%d  -  %d", ha, hb), 28);
            DrawText(TextFormat("%d  -  %d", ha, hb), (screenW - scoreW) / 2, y + 12, 28, textMain);
            int rw = MeasureText(teams[b].name, 22);
            DrawText(teams[b].name, screenW - rw - 56, y + 16, 22, textMain);

            DrawCircle(68, y + 52, 5, live);
            DrawText("LIVE'", 80, y + 44, 18, live);

            y += 84;
            row++;
        }
        DrawText("Scores echo each club's goals tally (mod 5) for a quick demo readout.", 40, screenH - 72, 18, GetColor(0x6B7280FF));
    }

    if (goBack) *goBack = back;
}

void DrawMatchScheduleScreen(const std::vector<Team>& teams, Vector2 mouse, int screenW, int screenH, bool* goBack) {
    bool back = false;
    SubScreenBackground(screenW, screenH);
    DrawSubScreenChrome("Match schedule", mouse, screenW, screenH, &back);

    int y = 76;
    Color textMain = GetColor(0x111827FF);
    const char* days[] = {"Sat 15:00", "Sat 17:30", "Sun 14:00", "Sun 16:30", "Mon 20:00"};

    if (teams.size() < 2) {
        DrawText("Add teams in League table to populate the fixture list.", 40, y + 20, 22, textMain);
    } else {
        int n = (int)teams.size();
        int k = 0;
        for (int r = 0; r < n - 1 && k < 5; r++) {
            int home = r % n;
            int away = (r + 1 + (k / 2)) % n;
            if (home == away) away = (away + 1) % n;
            Rectangle box{40, (float)y, (float)screenW - 80, 64};
            DrawRectangleRounded(box, 0.1f, 8, WHITE);
            DrawRectangleRoundedLinesEx(box, 0.1f, 8, 1.5f, GetColor(0xD1D5DBFF));
            DrawText(days[k % 5], 52, y + 20, 18, GetColor(0x6B7280FF));
            const char* line = TextFormat("%s  vs  %s", teams[home].name, teams[away].name);
            DrawText(line, 220, y + 18, 20, textMain);
            DrawText("MD12", screenW - 120, y + 20, 18, GetColor(0x15803DFF));
            y += 72;
            k++;
        }
    }

    if (goBack) *goBack = back;
}

static const char* TierLabel(int points) {
    if (points >= 18) return "Elite";
    if (points >= 10) return "Strong";
    if (points >= 4) return "Rising";
    return "Developing";
}

void DrawSeasonStatsScreen(const std::vector<Team>& teams, Vector2 mouse, int screenW, int screenH, bool* goBack) {
    bool back = false;
    SubScreenBackground(screenW, screenH);
    DrawSubScreenChrome("Season stats", mouse, screenW, screenH, &back);

    int y = 80;
    Color textMain = GetColor(0x111827FF);

    int totalGoals = calculate_total_goals_from_teams_recursive(teams, (int)teams.size());
    int totalPts = calculate_total_points_recursive(teams, (int)teams.size());
    int matchGoalsTotal = calculate_demo_match_goals_total_logic(teams);
    float avgGoals = teams.empty() ? 0.0f : (float)totalGoals / (float)teams.size();

    const Team* best = nullptr;
    for (const auto& t : teams) {
        if (!best || t.points > best->points || (t.points == best->points && t.goalsScored > best->goalsScored)) {
            best = &t;
        }
    }

    DrawText("League totals", 48, y, 26, GetColor(0x14532DFF));
    y += 40;
    DrawText(TextFormat("Clubs registered: %d", (int)teams.size()), 56, y, 22, textMain);
    y += 32;
    DrawText(TextFormat("Goals recorded (season): %d", totalGoals), 56, y, 22, textMain);
    y += 32;
    DrawText(TextFormat("Points in table (sum): %d", totalPts), 56, y, 22, textMain);
    y += 32;
    DrawText(TextFormat("Average goals per club: %.1f", avgGoals), 56, y, 22, textMain);
    y += 32;
    DrawText(TextFormat("Demo fixture goals (recursive): %d", matchGoalsTotal), 56, y, 22, textMain);
    y += 44;

    if (best) {
        DrawText("Table leader", 48, y, 26, GetColor(0x14532DFF));
        y += 36;
        DrawText(
            TextFormat("%s — %d pts, %d goals (%s)", best->name, best->points, best->goalsScored, TierLabel(best->points)),
            56, y, 22, textMain);
        y += 40;
    }

    DrawText("Tip: edit clubs in League table to refresh these aggregates.", 48, screenH - 64, 18, GetColor(0x6B7280FF));

    if (goBack) *goBack = back;
}

void DrawAboutScreen(Vector2 mouse, int screenW, int screenH, bool* goBack) {
    bool back = false;
    SubScreenBackground(screenW, screenH);
    DrawSubScreenChrome("About", mouse, screenW, screenH, &back);

    int y = 88;
    Color textMain = GetColor(0x111827FF);
    DrawText("Pitch & Score", 48, y, 32, GetColor(0x14532DFF));
    y += 44;
    DrawText("A raylib desk app for tracking football-style league tables.", 48, y, 20, textMain);
    y += 32;
    DrawText("League table: add clubs, sort, and adjust points and goals.", 48, y, 20, textMain);
    y += 28;
    DrawText("Other screens preview live boards, fixtures, and season rollups.", 48, y, 20, textMain);
    y += 36;
    DrawText("Algorithms (back-end):", 48, y, 22, GetColor(0x14532DFF));
    y += 28;
    DrawText("- Quick Sort by points | std::sort for goals and name", 56, y, 18, textMain);
    y += 24;
    DrawText("- Linear + Binary Search (name sort) + partial name match", 56, y, 18, textMain);
    y += 24;
    DrawText("- Recursion: total goals, points, and demo match series", 56, y, 18, textMain);
    y += 36;
    DrawText("Three-tier: presentation -> logic -> data (database.bin)", 48, y, 18, textMain);
    y += 28;
    DrawText("BinaryBeasts — built with raylib", 48, y, 18, GetColor(0x6B7280FF));

    if (goBack) *goBack = back;
}
