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

int calculate_total_goals_recursive(const std::vector<Match>& matches, int n) {
    if (n <= 0) return 0;
    return (matches[n - 1].scoreA + matches[n - 1].scoreB) + calculate_total_goals_recursive(matches, n - 1);
}

int calculate_total_goals_from_teams_recursive(const std::vector<Team>& teams, int n) {
    if (n <= 0) return 0;
    return teams[n - 1].goalsScored + calculate_total_goals_from_teams_recursive(teams, n - 1);
}

int find_team_by_name_linear_logic(const std::vector<Team>& teams, const char* name) {
    if (name == nullptr || name[0] == '\0') return -1;
    for (int i = 0; i < (int)teams.size(); ++i) {
        if (strcmp(teams[i].name, name) == 0) return i;
    }
    return -1;
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