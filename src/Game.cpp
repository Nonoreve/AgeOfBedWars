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

#define ARGUMENTS 4
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
		while (true) { // TODO ai
			std::cout << "Enter the name of your target" << std::endl; // TODO print player names
			string targetName;
			std::cin >> targetName;
			std::transform(targetName.begin(), targetName.end(), targetName.begin(),
			               [](unsigned char c) { return std::tolower(c); });
			if (targetName == curPlName) {
				std::cout << "You cannot target yourself." << std::endl;
				continue;
			}
			bool targetFound = false;
			auto it = players.begin();
			while (!targetFound && it != players.end()) {
				string plName = it->getName();
				std::transform(plName.begin(), plName.end(), plName.begin(),
				               [](unsigned char c) { return std::tolower(c); });
				if (plName == targetName)
					targetFound = true;
				++it;
			}
			if (targetFound) {
				return terrain.getBase((int) std::distance(players.begin(), it - 1));
			} else {
				std::cout << "This is not the name of any player. Try again." << std::endl;
			}
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
		std::cerr
				<< "Usage : `AgeOfBedWars <backgroundFile> <baseHealth> <moneyPerTurn> <roundsBeforeGameOver> [initialMoneyAmount]`"
				<< std::endl;
		std::cerr << "\t<arguments> are required. [arguments] are optionnal." << std::endl;
		return -1;
	}
	const string backgroundFilename(argv[1]);
	const int baseHealth = std::stoi(argv[2]);
	const int moneyPerTurn = std::stoi(argv[3]);
	const int roundsBeforeGameOver = std::stoi(argv[4]);

	int initialMoneyAmount = 0;
	if (argc > ARGUMENTS + 1) {
		initialMoneyAmount = std::stoi(argv[5]);
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
		bool validForm = false;
		string name;
		while (!validForm) {
			getline(std::cin, name);
			std::cin.clear();
			validForm = (!name.empty() && name != "\n");
		}
		std::cout << "Is " << name << " an ai ? (Y|n) : ";
		validForm = false;
		char yesNo;
		while(!validForm) {
			std::cin >> yesNo;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (std::toupper(yesNo) == 'Y') {
				players.emplace_back(name, initialMoneyAmount, terrain.getBaseInCreatedOrder(name), true);
				i++;
				validForm = true;
			} else if (std::toupper(yesNo) == 'N') {
				players.emplace_back(name, initialMoneyAmount, terrain.getBaseInCreatedOrder(name));
				i++;
				validForm = true;
			} else
				std::cout << R"(Expected answer is "Y" or "n".)" << std::endl;
		}
	}

	std::cout << std::endl << " - - - - - Age of Bed Wars - - - - -\n" << std::endl;
	int round = 0;
	int loser = -1;
	while (loser == -1 && round < roundsBeforeGameOver) { // TODO progress bar for remaining rounds
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