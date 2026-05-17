#define _CRT_SECURE_NO_WARNINGS
#include "logic.h"
#include <cstring>
#include <algorithm>
#include <cstdlib>

using namespace std;

static int partitionPointsDesc(vector<Team>& teams, int low, int high) {
    int pivot = teams[high].points;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (teams[j].points > pivot) {
            i++;
            swap(teams[i], teams[j]);
        }
    }
    swap(teams[i + 1], teams[high]);
    return (i + 1);
}

static void quickSortPointsDesc(vector<Team>& teams, int low, int high) {
    if (low < high) {
        int pi = partitionPointsDesc(teams, low, high);
        quickSortPointsDesc(teams, low, pi - 1);
        quickSortPointsDesc(teams, pi + 1, high);
    }
}

void addTeamLogic(vector<Team>& teams, const char* name) {
    if (strlen(name) < 1) return;
    Team t;
    t.id = (int)teams.size() + 1;
    strncpy(t.name, name, 49);
    t.name[49] = '\0';
    t.points = 0;
    t.goalsScored = 0;
    teams.push_back(t);
}

void deleteLastTeamLogic(vector<Team>& teams) {
    if (!teams.empty()) teams.pop_back();
}

void clearAllTeamsLogic(vector<Team>& teams) {
    teams.clear();
}

void persistTeamsLogic(const vector<Team>& teams) {
    saveData(teams);
}

static bool sanitizeTeamNames(vector<Team>& teams) {
    bool changed = false;
    for (auto& t : teams) {
        if (strcmp(t.name, "Bayern Muchen") == 0) {
            strncpy(t.name, "Bayern Munchen", sizeof(t.name) - 1);
            t.name[sizeof(t.name) - 1] = '\0';
            changed = true;
        }
    }
    return changed;
}

vector<Team> loadTeamsLogic() {
    vector<Team> teams = loadData();
    if (sanitizeTeamNames(teams)) {
        persistTeamsLogic(teams);
    }
    return teams;
}

int calculateTotalGoalsRecursive(const vector<Match>& matches, int n) {
    if (n <= 0) return 0;
    return (matches[n - 1].scoreA + matches[n - 1].scoreB) + calculateTotalGoalsRecursive(matches, n - 1);
}

int calculateTotalGoalsFromTeamsRecursive(const vector<Team>& teams, int n) {
    if (n <= 0) return 0;
    return teams[n - 1].goalsScored + calculateTotalGoalsFromTeamsRecursive(teams, n - 1);
}

static void toLowerCopy(const char* src, char* dst, int dstSize) {
    int i = 0;
    for (; src[i] != '\0' && i < dstSize - 1; ++i) {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') c = (char)(c + 32);
        dst[i] = c;
    }
    dst[i] = '\0';
}

static int nameContainsIgnoreCase(const char* teamName, const char* query) {
    if (query == nullptr || query[0] == '\0') return 0;
    char nameBuf[64];
    char queryBuf[64];
    toLowerCopy(teamName, nameBuf, (int)sizeof(nameBuf));
    toLowerCopy(query, queryBuf, (int)sizeof(queryBuf));
    return strstr(nameBuf, queryBuf) != nullptr;
}

int findTeamByNameLinearLogic(const vector<Team>& teams, const char* name) {
    if (name == nullptr || name[0] == '\0') return -1;
    for (int i = 0; i < (int)teams.size(); ++i) {
        if (strcmp(teams[i].name, name) == 0) return i;
    }
    return -1;
}

int findTeamByNameBinaryLogic(const vector<Team>& teams, const char* name) {
    if (name == nullptr || name[0] == '\0') return -1;
    int low = 0;
    int high = (int)teams.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(teams[mid].name, name);
        if (cmp == 0) return mid;
        if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int findTeamByNamePartialLogic(const vector<Team>& teams, const char* query) {
    if (query == nullptr || query[0] == '\0') return -1;
    for (int i = 0; i < (int)teams.size(); ++i) {
        if (nameContainsIgnoreCase(teams[i].name, query)) return i;
    }
    return -1;
}

int calculateTotalPointsRecursive(const vector<Team>& teams, int n) {
    if (n <= 0) return 0;
    return teams[n - 1].points + calculateTotalPointsRecursive(teams, n - 1);
}

int calculateDemoMatchGoalsTotalLogic(const vector<Team>& teams) {
    vector<Match> matches;
    int n = (int)teams.size();
    if (n < 2) return 0;
    int pairCount = n - 1;
    if (pairCount > 6) pairCount = 6;
    for (int i = 0; i < pairCount; ++i) {
        Match m;
        m.scoreA = teams[i].goalsScored % 5;
        m.scoreB = teams[(i + 1) % n].goalsScored % 5;
        matches.push_back(m);
    }
    return calculateTotalGoalsRecursive(matches, (int)matches.size());
}

void sortTeamsByModeLogic(vector<Team>& teams, int sortMode) {
    if (teams.size() <= 1) return;

    if (sortMode == 0) {
        quickSortPointsDesc(teams, 0, (int)teams.size() - 1);
    } else if (sortMode == 1) {
        sort(teams.begin(), teams.end(), [](const Team& a, const Team& b) {
            if (a.goalsScored == b.goalsScored) return a.points > b.points;
            return a.goalsScored > b.goalsScored;
        });
    } else {
        sort(teams.begin(), teams.end(), [](const Team& a, const Team& b) {
            return strcmp(a.name, b.name) < 0;
        });
    }
}
