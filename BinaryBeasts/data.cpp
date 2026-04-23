#define _CRT_SECURE_NO_WARNINGS
#include "data.h"
#include <fstream>

void save_data(const std::vector<Team>& teams) {
    std::ofstream file("database.bin", std::ios::binary | std::ios::trunc);
    if (file.is_open()) {
        for (const auto& t : teams) {
            file.write((const char*)&t, sizeof(Team));
        }
        file.close();
    }
}

std::vector<Team> load_data() {
    std::vector<Team> teams;
    std::ifstream file("database.bin", std::ios::binary);
    if (!file.is_open()) return teams;

    Team t;
    while (file.read((char*)&t, sizeof(Team))) {
        teams.push_back(t);
    }
    file.close();
    return teams;
}