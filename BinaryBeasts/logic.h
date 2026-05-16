#ifndef LOGIC_H
#define LOGIC_H

#include "data.h"
#include <vector>

void add_team_logic(std::vector<Team>& teams, const char* name);
void sort_teams_by_mode_logic(std::vector<Team>& teams, int sortMode);
int find_team_by_name_linear_logic(const std::vector<Team>& teams, const char* name);
int find_team_by_name_binary_logic(const std::vector<Team>& teams, const char* name);
int find_team_by_name_partial_logic(const std::vector<Team>& teams, const char* query);
int calculate_total_goals_recursive(const std::vector<Match>& matches, int n);
int calculate_total_goals_from_teams_recursive(const std::vector<Team>& teams, int n);
int calculate_total_points_recursive(const std::vector<Team>& teams, int n);
int calculate_demo_match_goals_total_logic(const std::vector<Team>& teams);
void delete_last_team_logic(std::vector<Team>& teams);
void clear_all_teams_logic(std::vector<Team>& teams);
void persist_teams_logic(const std::vector<Team>& teams);
std::vector<Team> load_teams_logic();

#endif