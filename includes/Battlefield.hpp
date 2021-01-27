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
	vector<vector<char>> _background; // not a vector<string> because we want to safely modify it
	// tell how many cell there is between two bases
	std::pair<int, int> _cellsGrid;
	vector<Base> _bases;
	static int _baseIndex;

	static void printBackground(vector<vector<char>> &terrain);

	// background coordinates
	static void editTerrainAt(vector<vector<char>> &terrain, std::pair<int, int> pos, char tileToPut);

	// background coordinates
	static void editTerrainAt(vector<vector<char>> &terrain, int posX, int posY, char tileToPut);

public:
	Battlefield(const string &filename, int baseHealth);

	inline int loadedBases() {
		return _bases.size();
	};

	/**
	 * The bases are created and attached in the order of the Positions in the file.
	 * @return
	 */
	inline Base &getBaseInCreatedOrder() {
		return _bases.at(++_baseIndex); // TODO check incrementation applies after object retrieval
	}

	/**
	 *
	 * @return The index of the first destroyed base found or -1.
	 */
	int basesStatus(); // TODO move to Base

	/**
	 * Check if the cell at the given position has no Unit on it.
	 * @param position
	 * @return
	 */
	bool isCellFree(Position position) const;

	void hitThere(Position position, int damages);

	/**
	 * Draw the background and all Units above
	 */
	void drawTerrain();

	inline Base &getBase(int index) {
		return _bases.at(index);
	}

};

#endif //AGEOFBEDWARS_BATTLEFIELD_HPP
