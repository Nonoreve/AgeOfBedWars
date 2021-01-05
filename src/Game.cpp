/**
 * Game.cpp : Main file responsible for arguments parsing, Battlefield and Players instacing and running game loop.
 * Created by nonoreve on 1/5/21.
 */

#include "Player.hpp"
#include "Battlefield.hpp"

#include <iostream>
#include <array>

#define PLAYERS 2
#define ARGUMENTS 1

using std::string;

int main(int argc, char *argv[]) {
    if (argc != ARGUMENTS + 1) {
        std::cerr << "Invalid number of arguments. Expected " << ARGUMENTS << " got " << argc - 1 << "." << std::endl;
        std::cerr << "Usage : `AgeOfBedWars <backgroundFile>`" << std::endl;
        std::cerr << "\t<arguments> are required. [arguments] are optionnal." << std::endl;
        return -1;
    }
    const string backgroundFilename(argv[1]);

    int round = 0;
    Battlefield terrain(backgroundFilename);
    std::array<Player, PLAYERS> players = {};
    return 0;
}
