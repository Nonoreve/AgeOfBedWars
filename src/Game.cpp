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
#include <chrono>
#include <thread>
#include <random>

#define ARGUMENTS 3
#define OPTIONALARGUMENTS 1

template<class Iterator>
Base &opponentBase(const std::vector<Player> &players, Iterator currentPlayer, Battlefield &terrain) {
	if (players.size() == 2) {
		auto otherPlayer = currentPlayer == players.begin() ? currentPlayer + 1 : players.begin();
		return terrain.getBase(std::distance(players.begin(), otherPlayer));
	} else {
		// discard all the uppercases
		string curPlName = currentPlayer->getName(); // copy
		std::transform(curPlName.begin(), curPlName.end(), curPlName.begin(),
		               [](unsigned char c) { return std::tolower(c); });
		while (true) {
			std::cout << "Enter the name of your target" << std::endl;
			string targetName;
			std::cin >> targetName;
			std::transform(targetName.begin(), targetName.end(), targetName.begin(),
			               [](unsigned char c) { return std::tolower(c); });
			bool targetFound = false;
			auto it = players.begin();
			while (!targetFound && it != players.end()) {
				string plName = it->getName();
				std::transform(targetName.begin(), targetName.end(), targetName.begin(),
				               [](unsigned char c) { return std::tolower(c); });
				if (plName == targetName && targetName != curPlName)
					targetFound = true;
				++it;
			}
			if (targetFound) {
				return terrain.getBase(std::distance(players.begin(), it));
			} else std::cout << "This is not the name of any player. Try again." << std::endl;
		}
	}

}

auto prices() {
	std::ostringstream prices;
	prices << "Available Units : " << std::endl;
	prices << "\tInfantry : $" << Player::UNIT_PRICES[0];
	prices << "\tArcher : $" << Player::UNIT_PRICES[1];
	prices << "\tCatapult : $" << Player::UNIT_PRICES[2];
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
	std::random_device rdev;
	std::srand(rdev());

	UnitPool unitPool;
	Battlefield terrain(unitPool, backgroundFilename, baseHealth);
	std::vector<Player> players;

	std::cout << "Terrain is for " << terrain.loadedBases() << " players." << std::endl;
	int i = 0;
	while (i < terrain.loadedBases()) {
		std::cout << "Enter name for player " << i << " : ";
		string name;
		if (getline(std::cin, name)) {
			std::cin.clear();
//			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush stream
			std::cout << "Is " << name << " an ai ? (Y|n) : ";
			string yesNo;
//			std::cin >> yesNo;
			getline(std::cin, yesNo);
			std::cin.clear();
//			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (std::toupper(yesNo[0]) == 'Y') {
				players.emplace_back(name, initialMoneyAmount, terrain.getBaseInCreatedOrder(name), true);
				i++;
			}
			else if (std::toupper(yesNo[0]) == 'N') {
				players.emplace_back(name, initialMoneyAmount, terrain.getBaseInCreatedOrder(name));
				i++;
			} else
				std::cout << R"(Expected answer is "Y" or "n".)" << std::endl;
		} else {
			std::cout << "Invalid name. Try again." << std::endl;
		}
	}

	std::cout << std::endl << " - - - - - Age of Bed Wars - - - - -\n" << std::endl;
	int round = 0;
	int loser = -1;
	while (loser == -1 && round < 100) { // TODO rounds as argument and progressive bar
		std::cout << std::endl << std::endl << "Round number " << round << std::endl;
		// distributes money for all the players
		std::for_each(players.begin(), players.end(), [&](Player &p) { p.pay(moneyPerTurn); });
		auto currentPlayer = players.begin();
		while (currentPlayer != players.end() && loser == -1) {
			std::cout << std::endl << "Its the turn of " << currentPlayer->getName() << std::endl;
			// all 3 action phases
			terrain.playActions(*currentPlayer);
			// check for a loser
			loser = terrain.basesStatus();
			if (loser == -1) {
				std::cout << prices() << std::endl << std::endl;
				std::cout << currentPlayer->report() << std::endl;

				// summoning phase
				if (unitPool.isCellFree(currentPlayer->getBase().getPosition())) {
					bool validForm = false;
					while (!validForm) {
						UnitType unitType = currentPlayer->chooseUnit();
						if (unitType != UNKNOWN) {
							if (currentPlayer->canAfford(Player::UNIT_PRICES[unitType])) {
								if (unitPool.unitFactory(unitType, *currentPlayer,
								                         opponentBase(players, currentPlayer, terrain)) == nullptr) {
									std::cerr << "Unit summoning failed. " << std::endl;
									std::exit(-1);
								} else {
									currentPlayer->buy(unitType);
									validForm = true;
								}
							} else {
								std::cout << "Can't afford this unit. " << currentPlayer->report() << std::endl;
							}
						} else {
							validForm = true;
						}
					}
				} else {
					std::cout << "You can't summon for now your base is occupied. Skipping in 3s." << std::endl;
					std::chrono::milliseconds skipTime(3000);
					std::this_thread::sleep_for(skipTime);
				}
				++currentPlayer;
			}
		}
		round++;
	}
	if (round < 100)
		std::cout << "The loser is " << players.at(loser).getName(); // TODO look for winner
	else
		std::cout << "Game Over";
	std::cout << " (press any key)" << std::endl;
	std::cin.get();
	return 0;
}