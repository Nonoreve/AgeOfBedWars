//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_BATTLEFIELD_HPP
#define AGEOFBEDWARS_BATTLEFIELD_HPP

#include "Base.hpp"

#include <string>
#include <vector>
#include <units/UnitPool.hpp>

using std::string, std::vector;

class Battlefield {
private:
	vector<vector<char>> _background; // not a vector<string> because we want to safely modify it
	// tell how many cells there are between two bases
	std::pair<int, int> _cellsGrid;
	vector<Base> _bases;
	UnitPool &_unitPool;

	void doActionPhase(int actionPhase, Unit *unit);

	void buryCorpses();

	static int _baseIndex;

	static void printBackground(vector<vector<char>> &terrain);

	// background coordinates
	static void editTerrainAt(vector<vector<char>> &terrain, int posX, int posY, char tileToPut);

public:
	Battlefield(UnitPool &unitPool, const string &filename, int baseHealth);

	inline int loadedBases() {
		return _bases.size();
	};

	/**
	 * The bases are created and attached in the order of the Positions in the file.
	 * PlayerName is requested to edit the Base marks
	 * @return
	 */
	Base &getBaseInCreatedOrder(const string &playerName);

	/**
	 *
	 * @return The index of the first destroyed base found or -1.
	 */
	int basesStatus();

	/**
	 * Process the right action for all the units.
	 * @param currentPlayer the player who to moves its units
	 */
	void playActions(Player &currentPlayer);

	/**
	 * Only target Units (not bases)
	 * @param position
	 * @param damages
	 * @return the type of the unit if it was killed, UNKNOWN otherwise
	 */
	UnitType hitThere(Position position, int damages);

	/**
	 * Draw the background and all Units above
	 */
	void drawTerrain();

	inline Base &getBase(int index) {
		return _bases.at(index);
	}

};

#endif //AGEOFBEDWARS_BATTLEFIELD_HPP
