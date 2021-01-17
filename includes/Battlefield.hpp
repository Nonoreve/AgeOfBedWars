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
    // Not a member of Base because its relative to the background itself.
    // Position is in background coordinates.
    vector<std::pair<int, int>> basesPositions;
    vector<Base> bases;
    static int baseIndex;

    void printBackground();

    // background coordinates
    char terrainAt(std::pair<int, int> pos);

    // background coordinates
    char terrainAt(int posX, int posY);

public:
    explicit Battlefield(const string &filename);

    inline int loadedBases() { return basesPositions.size(); };

    /**
     * The bases are created an attached in the order of the loadedBases
     * @param health
     * @return
     */
    Base &createBase(int health);

    /**
     *
     * @return The index of the first destroyed base found or -1.
     */
    int basesStatus();

    /**
     *
     * @param index
     * @return
     */
    Base getBase(int index);

};

#endif //AGEOFBEDWARS_BATTLEFIELD_HPP
