/**
 * Game.cpp : Main file responsible for arguments parsing, Battlefield and Players instacing and running game loop.
 * Created by nonoreve on 1/5/21.
 */

#include "Player.hpp"
#include "Battlefield.hpp"

#include <iostream>

#define ARGUMENTS 3
#define OPTIONALARGUMENTS 0

using std::string;

int main(int argc, char *argv[]) {
    if (argc < ARGUMENTS + 1 || argc > ARGUMENTS + 1 + OPTIONALARGUMENTS) {
        std::cerr << "Invalid number of arguments. Expected " << ARGUMENTS << " got " << argc - 1 << "." << std::endl;
        std::cerr << "Usage : `AgeOfBedWars <backgroundFile> <initialMoneyAmount> <health>`" << std::endl;
        std::cerr << "\t<arguments> are required. [arguments] are optionnal." << std::endl;
        return -1;
    }
    const string backgroundFilename(argv[1]);
    const int initialMoneyAmount = std::stoi(argv[2]);
    const int health = std::stoi(argv[3]);

    int round = 0;
    Battlefield terrain(backgroundFilename);
    std::cout << "Terrain is for " << terrain.loadedBasePositions() << " players." << std::endl;
    std::vector<Player> players;
    int i = 0;
    while (i < terrain.loadedBasePositions()) {
        std::cout << "Enter name for player " << i << " : ";
        string name;
        if (std::cin >> name) {
            players.emplace_back(name, initialMoneyAmount, terrain.createBase(health));
            i++;
        } else {
            std::cout << "Invalid name. Try again." << std::endl;
        }
    }
    return 0;
}
