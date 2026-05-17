#define _CRT_SECURE_NO_WARNINGS
#include "data.h"
#include <fstream>

using namespace std;

void saveData(const vector<Team>& teams) {
    ofstream file("database.bin", ios::binary | ios::trunc);
    if (file.is_open()) {
        for (const auto& t : teams) {
            file.write((const char*)&t, sizeof(Team));
        }
        file.close();
    }
}

vector<Team> loadData() {
    vector<Team> teams;
    ifstream file("database.bin", ios::binary);
    if (!file.is_open()) return teams;

    Team t;
    while (file.read((char*)&t, sizeof(Team))) {
        teams.push_back(t);
    }
    file.close();
    return teams;
}
