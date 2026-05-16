#define _CRT_SECURE_NO_WARNINGS
#include "logic.h"
#include <cstring>
#include <algorithm>
#include <cstdlib>

static int partition_points_desc(std::vector<Team>& teams, int low, int high) {
    int pivot = teams[high].points;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (teams[j].points > pivot) {
            i++;
            std::swap(teams[i], teams[j]);
        }
    }
    std::swap(teams[i + 1], teams[high]);
    return (i + 1);
}

static void quick_sort_points_desc(std::vector<Team>& teams, int low, int high) {
    if (low < high) {
        int pi = partition_points_desc(teams, low, high);
        quick_sort_points_desc(teams, low, pi - 1);
        quick_sort_points_desc(teams, pi + 1, high);
    }
}

void add_team_logic(std::vector<Team>& teams, const char* name) {
    if (strlen(name) < 1) return;
    Team t;
    t.id = (int)teams.size() + 1;
    strncpy(t.name, name, 49);
    t.name[49] = '\0';
    t.points = 0;
    t.goalsScored = 0;
    teams.push_back(t);
}

void delete_last_team_logic(std::vector<Team>& teams) {
    if (!teams.empty()) teams.pop_back();
}

void clear_all_teams_logic(std::vector<Team>& teams) {
    teams.clear();
}

void persist_teams_logic(const std::vector<Team>& teams) {
    save_data(teams);
}

std::vector<Team> load_teams_logic() {
    return load_data();
}

int calculate_total_goals_recursive(const std::vector<Match>& matches, int n) {
    if (n <= 0) return 0;
    return (matches[n - 1].scoreA + matches[n - 1].scoreB) + calculate_total_goals_recursive(matches, n - 1);
}

int calculate_total_goals_from_teams_recursive(const std::vector<Team>& teams, int n) {
    if (n <= 0) return 0;
    return teams[n - 1].goalsScored + calculate_total_goals_from_teams_recursive(teams, n - 1);
}

static void to_lower_copy(const char* src, char* dst, int dstSize) {
    int i = 0;
    for (; src[i] != '\0' && i < dstSize - 1; ++i) {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') c = (char)(c + 32);
        dst[i] = c;
    }
    dst[i] = '\0';
}

static int name_contains_ignore_case(const char* teamName, const char* query) {
    if (query == nullptr || query[0] == '\0') return 0;
    char nameBuf[64];
    char queryBuf[64];
    to_lower_copy(teamName, nameBuf, (int)sizeof(nameBuf));
    to_lower_copy(query, queryBuf, (int)sizeof(queryBuf));
    return strstr(nameBuf, queryBuf) != nullptr;
}

int find_team_by_name_linear_logic(const std::vector<Team>& teams, const char* name) {
    if (name == nullptr || name[0] == '\0') return -1;
    for (int i = 0; i < (int)teams.size(); ++i) {
        if (strcmp(teams[i].name, name) == 0) return i;
    }
    return -1;
}

int find_team_by_name_binary_logic(const std::vector<Team>& teams, const char* name) {
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

int find_team_by_name_partial_logic(const std::vector<Team>& teams, const char* query) {
    if (query == nullptr || query[0] == '\0') return -1;
    for (int i = 0; i < (int)teams.size(); ++i) {
        if (name_contains_ignore_case(teams[i].name, query)) return i;
    }
    return -1;
}

int calculate_total_points_recursive(const std::vector<Team>& teams, int n) {
    if (n <= 0) return 0;
    return teams[n - 1].points + calculate_total_points_recursive(teams, n - 1);
}

int calculate_demo_match_goals_total_logic(const std::vector<Team>& teams) {
    std::vector<Match> matches;
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
    return calculate_total_goals_recursive(matches, (int)matches.size());
}

void sort_teams_by_mode_logic(std::vector<Team>& teams, int sortMode) {
    if (teams.size() <= 1) return;

    if (sortMode == 0) {
        quick_sort_points_desc(teams, 0, (int)teams.size() - 1);
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