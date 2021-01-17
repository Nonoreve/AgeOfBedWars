//
// Created by nonoreve on 1/5/21.
//
#include "Battlefield.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <tuple>

/**
 * Extract all battlefield informations from filename.
 * The file should be written as follows :
 * (first line) size: integer, number of cells between the two bases
 * (second line) comma separated list of 2D coordinates
 * (third and more lines) caracter representation of the background of the battlefield
 */
Battlefield::Battlefield(const string &filename) {
    std::ifstream istrm(filename, std::ios::binary);
    if (!istrm.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
    } else {
        int size = 0;
        if (!(istrm >> size)) {
            std::cerr << "Error reading file : Background files should start by the size of the grid." << std::endl;
            std::exit(-1);
        }
        std::cout << "Size of the grid : " << size << std::endl;
        istrm.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        string line;
        std::getline(istrm, line);
        std::istringstream iss(line);
        std::cout << "Pos line : " << line << std::endl;
        int x, y;
        bool posOver = false;
        while (!posOver && iss >> x >> y) {
            std::cout << "New position : x=" << x << " y=" << y << std::endl;
            basesPositions.emplace_back(x, y);
            char comma = '\0';
            iss >> comma; // trying to read a comma
            posOver = comma == '\0'; // if nothing is read we reached the end of the line
            if (comma != ',' && !posOver) {
                std::cerr << "Error reading file : Positions should be separated by a comma ( , )." << std::endl;
                std::exit(-1);
            }
        }
        std::cout << "Finished reading " << basesPositions.size() << " basesPositions." << std::endl;

        while (!istrm.eof()) {
            getline(istrm, line);
            line.push_back('\n');
            background.push_back(line);
        }
        if (background.empty()) {
            std::cerr << "Error reading file : Background files should contain at least a string at line 3."
                      << std::endl;
            std::exit(-1);
        }

        std::for_each(basesPositions.begin(), basesPositions.end(), [&](auto e) {
            int x, y;
            std::tie(x, y) = e;
            if (x < 0 || x > background[0].size() || y < 0 || y > background.size()) {
                std::cerr << "Position out of terrain : " << x << ", " << y << std::endl;
                std::exit(-1);
            }
        });

        std::cout << "Read background : length=" << background[0].size() << " size=" << background.size() << std::endl;
        printBackground();
    }
};

int Battlefield::baseIndex = 0;

Base &Battlefield::createBase(int health) {
    bases.emplace_back(health);
    baseIndex++;
    return bases[baseIndex - 1];
}

void Battlefield::printBackground() {
    std::for_each(background.begin(), background.end(), [](const string &l) { std::cout << l; });
}

char Battlefield::terrainAt(std::pair<int, int> pos) {
    int x, y;
    std::tie(x, y) = pos;
    return terrainAt(x, y);
}

char Battlefield::terrainAt(int posX, int posY) {
    return background[posY][posX];
}

int Battlefield::basesStatus() {
    // TODO
    return -1;
}

Base Battlefield::getBase(int index) {
    return bases.at(index);
}
