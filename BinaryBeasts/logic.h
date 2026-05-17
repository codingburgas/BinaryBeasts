#ifndef LOGIC_H
#define LOGIC_H

#include "data.h"
#include <vector>

void addTeamLogic(std::vector<Team>& teams, const char* name);
void sortTeamsByModeLogic(std::vector<Team>& teams, int sortMode);
int findTeamByNameLinearLogic(const std::vector<Team>& teams, const char* name);
int findTeamByNameBinaryLogic(const std::vector<Team>& teams, const char* name);
int findTeamByNamePartialLogic(const std::vector<Team>& teams, const char* query);
int calculateTotalGoalsRecursive(const std::vector<Match>& matches, int n);
int calculateTotalGoalsFromTeamsRecursive(const std::vector<Team>& teams, int n);
int calculateTotalPointsRecursive(const std::vector<Team>& teams, int n);
int calculateDemoMatchGoalsTotalLogic(const std::vector<Team>& teams);
void deleteLastTeamLogic(std::vector<Team>& teams);
void clearAllTeamsLogic(std::vector<Team>& teams);
void persistTeamsLogic(const std::vector<Team>& teams);
std::vector<Team> loadTeamsLogic();

#endif
