//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_BATTLEFIELD_HPP
#define AGEOFBEDWARS_BATTLEFIELD_HPP

#include "Base.hpp"

#include <string>
#include <vector>

using std::string, std::vector;

class Battlefield {
private:
    vector<string> background;
    // not a member of Base because its relative to the background itself and not used in Base
    vector<std::pair<int, int>> basesPositions;
    vector<Base> bases;
    static int baseIndex;

    void printBackground();

    char terrainAt(std::pair<int, int> pos);

    char terrainAt(int posX, int posY);

public:
    explicit Battlefield(const string &filename);

    inline int loadedBasePositions() { return basesPositions.size(); };

    /**
     * The bases are created an attached in the order of the loadedBases
     * @param health
     * @return
     */
    Base& createBase(int health);

};


#endif //AGEOFBEDWARS_BATTLEFIELD_HPP
