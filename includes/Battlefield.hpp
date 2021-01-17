//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_BATTLEFIELD_HPP
#define AGEOFBEDWARS_BATTLEFIELD_HPP

#include "Base.hpp"

#include <string>
#include <vector>

using std::string, std::vector;

/**
 * Hold a 2D point in cell coordinates
 */
struct Position {
    int x;
    int y;

    /**
     *
     * @param x
     * @param y default to 0 for 1 dimensional games
     */
    explicit Position(int x, int y = 0) : x(x), y(y) {};

};

class Battlefield {
private:
    vector<string> background;
    // Not a member of Base because its relative to the background itself and not used in Base.
    // Position is in grid coordinates.
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
    Base &createBase(int health);

};

#endif //AGEOFBEDWARS_BATTLEFIELD_HPP
