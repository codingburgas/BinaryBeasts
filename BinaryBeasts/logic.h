#ifndef LOGIC_H
#define LOGIC_H

#include "data.h"
#include <vector>

void add_team_logic(std::vector<Team>& teams, const char* name);
void quick_sort_teams(std::vector<Team>& teams, int low, int high);
int calculate_total_goals_recursive(const std::vector<Match>& matches, int n);
void delete_last_team_logic(std::vector<Team>& teams);
void clear_all_teams_logic(std::vector<Team>& teams);

#endif