/**
 * Game.cpp : Main file responsible for arguments parsing, Battlefield and Players instacing and running game loop.
 * Created by nonoreve on 1/5/21.
 */

#include "units/UnitPool.hpp"
#include "Player.hpp"
#include "Battlefield.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>

#define ARGUMENTS 3
#define OPTIONALARGUMENTS 1

template<class Iterator>
Base opponentBase(const std::vector<Player> &players, Iterator currentPlayer, Battlefield &terrain) {
	if (players.size() == 2) {
		auto otherPlayer = currentPlayer == players.begin() ? currentPlayer + 1 : players.begin();
		return terrain.getBase(std::distance(players.begin(), otherPlayer));
	} else {
		// TODO ask current player which base to target
		std::cerr << "More than 2 players not supported at the moment." << std::endl;
		std::exit(-1);
	}

}

UnitType parseUnitName(string unitName) {
	std::transform(unitName.begin(), unitName.end(), unitName.begin(), [](unsigned char c) { return std::tolower(c); });
	if (unitName == "infantry")
		return UnitType::INFANTRYMAN;
	if (unitName == "archer")
		return UnitType::ARCHER;
	if (unitName == "catapult")
		return UnitType::CATAPULT;
	return UNKNOWN;
}

auto prices() {
	std::ostringstream prices;
	prices << "Available Units : " << std::endl;
	prices << "\tInfantry : $" << Unit::UNIT_PRICES[0];
	prices << "\tArcher : $" << Unit::UNIT_PRICES[1];
	prices << "\tCatapult : $" << Unit::UNIT_PRICES[2];
	return prices.str();
}

int main(int argc, char *argv[]) {
	if (argc < ARGUMENTS + 1 || argc > ARGUMENTS + 1 + OPTIONALARGUMENTS) {
		std::cerr << "Invalid number of arguments. Expected " << ARGUMENTS << " got " << argc - 1 << "." << std::endl;
		std::cerr << "Usage : `AgeOfBedWars <backgroundFile> <baseHealth> <moneyPerTurn> [initialMoneyAmount]`"
		          << std::endl;
		std::cerr << "\t<arguments> are required. [arguments] are optionnal." << std::endl;
		return -1;
	}
	const string backgroundFilename(argv[1]);
	const int baseHealth = std::stoi(argv[2]);
	const int moneyPerTurn = std::stoi(argv[3]);

	int initialMoneyAmount = 0;
	if (argc > ARGUMENTS + 1) {
		initialMoneyAmount = std::stoi(argv[4]);
	}

	UnitPool unitPool;
	Battlefield terrain(unitPool, backgroundFilename, baseHealth);
	std::vector<Player> players;

	std::cout << "Terrain is for " << terrain.loadedBases() << " players." << std::endl;
	int i = 0;
	while (i < terrain.loadedBases()) {
		std::cout << "Enter name for player " << i << " : ";
		string name;
		if (std::cin >> name) {
			players.emplace_back(name, initialMoneyAmount, terrain.getBaseInCreatedOrder(name));
			i++;
		} else {
			std::cout << "Invalid name. Try again." << std::endl;
		}
	}

	std::cout << std::endl << " - - - - - Age of Bed Wars - - - - -\n" << std::endl;
	int round = 0;
	int winner = -1;
	while (winner == -1 && round < 100) {
		std::cout << std::endl << "Round number " << round << std::endl;
		// all 3 action phases
		terrain.playActions();
		// distributes money for all the players
		std::for_each(players.begin(), players.end(), [&](Player &p) { p.pay(moneyPerTurn); });
		auto currentPlayer = players.begin();
		while (currentPlayer != players.end() && winner == -1) {
			terrain.drawTerrain();
			std::cout << "Turn of " << currentPlayer->getName() << "\n\t" << currentPlayer->report() << std::endl;
			std::cout << prices() << std::endl << std::endl;

			// summoning phase
			if (unitPool.isCellFree(currentPlayer->getBase().getPosition())) {
				bool validForm = false;
				while (!validForm) {
					string unit;
					std::cout << "Do you want to summon a new unit ? "
					             "(infantry|archer|catapult|none)" << std::endl;
					std::cin >> unit;
					UnitType unitType = parseUnitName(unit);
					if (unitType != UNKNOWN) {
						if (currentPlayer->canAfford(Unit::UNIT_PRICES[unitType])) {
							if (unitPool.unitFactory(unitType, currentPlayer->getBase().getPosition(),
							                         opponentBase(players, currentPlayer, terrain)) == nullptr) {
								std::cout << "Unit summoning failed. You have " << std::endl;
							} else {
								currentPlayer->buy(unitType);
								validForm = true;
							}
						} else {
							std::cout << "Can't afford this unit. You have " << currentPlayer->report()
							          << " money left." << std::endl;
						}
					} else if (unit == "none") {
						std::cout << "No unit summoned." << std::endl;
						validForm = true;
					} else {
						std::cout << "Wrong unit name : " << unit << ". Try again." << std::endl;
					}
				}
			} else {
				std::cout << "You can't summon for now your base is occupied." << std::endl;
			}
			// check for a winner
			winner = terrain.basesStatus();
			++currentPlayer;
		}
		round++;
	}
	if (round < 100)
		std::cout << "The winner is " << players.at(winner).getName() << std::endl;
	else
		std::cout << "Game Over" << std::endl;
	return 0;
}