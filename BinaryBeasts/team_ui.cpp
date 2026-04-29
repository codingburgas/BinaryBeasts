#include "team_ui.h"
#include <cstring>

int DrawTeamCard(const Team& team, int y, int cardWidth, int cardHeight, int nameX, int pointsX, int goalsX, int editX, bool showEdit, int fontSize, Vector2 mouse, bool* editClicked) {
    // Card shadow
    DrawRectangleRounded(Rectangle{(float)nameX-60+4, (float)y+8, (float)cardWidth-8, (float)cardHeight-8}, 0.22f, 12, Fade(BLACK, 0.08f));
    // Card gradient
    Color grad1 = GetColor(0xF4F7FAFF);
    Color grad2 = GetColor(0xE3EFFFEE);
    DrawRectangleRounded(Rectangle{(float)nameX-60, (float)y, (float)cardWidth, (float)cardHeight}, 0.22f, 12, grad1);
    DrawRectangleRounded(Rectangle{(float)nameX-60, (float)y+cardHeight/2, (float)cardWidth, (float)cardHeight/2}, 0.22f, 12, grad2);
    // Avatar
    DrawCircle(nameX-25, y + cardHeight/2, 26, GetColor(0x4ECCA3FF));
    char initials[3] = {0};
    initials[0] = team.name[0];
    for (int j = 1, k = 1; team.name[j] != '\0' && k < 2; j++) {
        if (team.name[j-1] == ' ' && team.name[j] != ' ') {
            initials[k++] = team.name[j];
        }
    }
    DrawText(initials, nameX-35, y + cardHeight/2 - 14, 28, DARKBLUE);
    // Team name (fit font size and truncate)
    int maxNameW = pointsX - nameX - 24;
    int nameFont = fontSize;
    char nameBuf[60];
    strncpy_s(nameBuf, sizeof(nameBuf), team.name, sizeof(nameBuf)-1); nameBuf[sizeof(nameBuf)-1] = '\0';
    int textW = MeasureText(nameBuf, nameFont);
    int nameLen = (int)strlen(nameBuf);
    while (textW > maxNameW && nameFont > 16) {
        nameFont--;
        textW = MeasureText(nameBuf, nameFont);
    }
    while (textW > maxNameW && nameLen > 3) {
        nameBuf[--nameLen] = '\0';
        int ellipIdx = (nameLen-3 > 0 ? nameLen-3 : 0);
        strcpy_s(&nameBuf[ellipIdx], sizeof(nameBuf)-ellipIdx, "...");
        textW = MeasureText(nameBuf, nameFont);
    }
    DrawText(nameBuf, nameX, y + cardHeight/2 - nameFont/2, nameFont, GetColor(0x232946FF));
    // Points
    DrawText(TextFormat("%d", team.points), pointsX, y + cardHeight/2 - fontSize/2, fontSize, GOLD);
    // Goals (fit in box)
    char goalsBuf[16];
    snprintf(goalsBuf, 15, "%d", team.goalsScored);
    int goalsW = MeasureText(goalsBuf, fontSize);
    int maxGoalsW = 80;
    int goalsFont = fontSize;
    while (goalsW > maxGoalsW && goalsFont > 16) {
        goalsFont--;
        goalsW = MeasureText(goalsBuf, goalsFont);
    }
    DrawText(goalsBuf, goalsX, y + cardHeight/2 - goalsFont/2, goalsFont, GetColor(0x4ECCA3FF));
    // Edit button
    bool clicked = false;
    if (showEdit) {
        Rectangle editBtn = Rectangle{(float)editX, (float)y + cardHeight/2 - 18, 36, 36};
        DrawRectangleRounded(editBtn, 0.4f, 8, GetColor(0xB0B8C1FF));
        DrawRectangle(editBtn.x+10, editBtn.y+20, 16, 4, GetColor(0x232946FF));
        DrawRectangle(editBtn.x+18, editBtn.y+10, 4, 16, GetColor(0x4ECCA3FF));
        if (CheckCollisionPointRec(mouse, editBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            clicked = true;
        }
    }
    if (editClicked) *editClicked = clicked;
    return cardHeight + 16;
}

int DrawTeamList(const std::vector<Team>& teams, int startY, int cardWidth, int cardHeight, int nameX, int pointsX, int goalsX, int editX, int fontSize, Vector2 mouse, int* editIdxClicked) {
    int y = startY;
    if (teams.empty()) {
        DrawText("No Teams Found", 600 - MeasureText("No Teams Found", 48)/2, 400, 48, GetColor(0xB0B8C1FF));
        return cardHeight;
    }
    for (size_t i = 0; i < teams.size(); ++i) {
        bool editClicked = false;
        DrawTeamCard(teams[i], y, cardWidth, cardHeight, nameX, pointsX, goalsX, editX, true, fontSize, mouse, &editClicked);
        if (editClicked && editIdxClicked) *editIdxClicked = (int)i;
        y += cardHeight + 16;
    }
    return y - startY;
}
