//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_BATTLEFIELD_HPP
#define AGEOFBEDWARS_BATTLEFIELD_HPP


#include <string>
#include <vector>

using std::string, std::vector;

class Battlefield {
private:
    vector<string> background;
    vector<std::pair<int, int>> positions;

    void printBackground();

public:
    explicit Battlefield(const string &filename);
};


#endif //AGEOFBEDWARS_BATTLEFIELD_HPP
