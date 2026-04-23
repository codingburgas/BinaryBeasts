#define _CRT_SECURE_NO_WARNINGS
#include "logic.h"
#include <cstring>
#include <algorithm>
#include <cstdlib>

void add_team_logic(std::vector<Team>& teams, const char* name) {
    if (strlen(name) < 1) return;
    Team t;
    t.id = (int)teams.size() + 1;
    strncpy(t.name, name, 49);
    t.name[49] = '\0';
    t.points = rand() % 100;
    t.goalsScored = rand() % 30;
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

int partition(std::vector<Team>& teams, int low, int high) {
    int pivot = teams[high].points;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (teams[j].points > pivot) { // Низходящ ред
            i++;
            std::swap(teams[i], teams[j]);
        }
    }
    std::swap(teams[i + 1], teams[high]);
    return (i + 1);
}

void quick_sort_teams(std::vector<Team>& teams, int low, int high) {
    if (low < high) {
        int pi = partition(teams, low, high);
        quick_sort_teams(teams, low, pi - 1);
        quick_sort_teams(teams, pi + 1, high);
    }
}