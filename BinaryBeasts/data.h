#ifndef DATA_H
#define DATA_H

#include <vector>

struct Team {
    int id;
    char name[50];
    int points;
    int goalsScored;
};

struct Match {
    int scoreA;
    int scoreB;
};

void saveData(const std::vector<Team>& teams);
std::vector<Team> loadData();

#endif
