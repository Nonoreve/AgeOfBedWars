/**
 * Game.cpp : Main file responsible for arguments parsing, Battlefield and Players instacing and running game loop.
 * Created by nonoreve on 1/5/21.
 */

#include "Player.hpp"
#include "Battlefield.hpp"

#include <iostream>
#include <algorithm>

#define ARGUMENTS 4
#define OPTIONALARGUMENTS 0

//using std::string;


template<class Iterator>
Base opponentBase(const std::vector<Player>& players, Iterator currentPlayer, const Battlefield& terrain) {
    if (players.size() == 2) {
        auto otherPlayer = currentPlayer == players.begin() ? currentPlayer + 1 : players.begin();
        return terrain.getBase(std::distance(players.begin(), otherPlayer));
    } else {
        // TODO ask current player which base to target
        std::cerr << "More than 2 players not supported at the moment." << std::endl;
        std::exit(-1);
    }

}

int main(int argc, char *argv[]) {
    if (argc < ARGUMENTS + 1 || argc > ARGUMENTS + 1 + OPTIONALARGUMENTS) {
        std::cerr << "Invalid number of arguments. Expected " << ARGUMENTS << " got " << argc - 1 << "." << std::endl;
        std::cerr << "Usage : `AgeOfBedWars <backgroundFile> <initialMoneyAmount> <health> <moneyPerTurn>`"
                  << std::endl;
        std::cerr << "\t<arguments> are required. [arguments] are optionnal." << std::endl;
        return -1;
    }
    const string backgroundFilename(argv[1]);
    const int initialMoneyAmount = std::stoi(argv[2]);
    const int health = std::stoi(argv[3]);
    const int moneyPerTurn = std::stoi(argv[4]);

    Battlefield terrain(backgroundFilename);
    std::vector<Player> players;

    std::cout << "Terrain is for " << terrain.loadedBases() << " players." << std::endl;
    int round = 0;

    int i = 0;
    while (i < terrain.loadedBases()) {
        std::cout << "Enter name for player " << i << " : ";
        string name;
        if (std::cin >> name) {
            players.emplace_back(name, initialMoneyAmount, terrain.createBase(health));
            i++;
        } else {
            std::cout << "Invalid name. Try again." << std::endl;
        }
    }

    std::cout << " - - - - - Age of Bed Wars - - - - -\n" << std::endl;
    int winner = -1;
    while (winner == -1) {
        // distributes money for all the players
        std::for_each(players.begin(), players.end(), [&](auto p) { p.pay(moneyPerTurn); });
        auto currentPlayer = players.begin();
        while (currentPlayer != players.end() && winner == -1) {
            std::cout << "Turn of ";
            currentPlayer->report();

            // all 3 action phases
            currentPlayer->play();

            // summoning phase
            if (currentPlayer->canSummon()) {
                string unit;
                bool validForm = false;
                while (!validForm) {
                    std::cout << "Do you want to summon a new unit ? "
                                 "(infantry|archer|catapult|none|[ENTER])" << std::endl;
                    std::cin >> unit;
                    if (unit == "infantry" || unit == "archer" || unit == "catapult") {
                        if (!currentPlayer->summon(unit, opponentBase(players, currentPlayer, terrain))) {
                            std::cout << "Unit summoning failed. You have " << currentPlayer->getMoney()
                                      << " money left." << std::endl;
                        } else
                            validForm = true;
                    } else if (unit == "none") { // TODO accept empty [ENTER] answer
                        std::cout << "No unit summoned." << std::endl;
                        validForm = true;
                    } else {
                        std::cout << "Wrong unit name : " << unit << ". Try again." << std::endl;
                    }
                }
            }
            ++currentPlayer;

            // check for a winner
            winner = terrain.basesStatus();
        }
        round++;
    }
    std::cout << "The winner is " << players.at(winner).getName() << std::endl;
    return 0;
}