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

void save_data(const std::vector<Team>& teams);
std::vector<Team> load_data();

#endif