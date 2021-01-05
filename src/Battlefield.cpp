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
        char comma = '\0';
        while (!posOver && iss >> x >> y) {
            std::cout << "New position : x=" << x << " y=" << y << std::endl;
            positions.emplace_back(x, y);
            iss >> comma; // trying to read a comma
            posOver = comma != '\0'; // if nothing is read we reached the end of the line
            if (comma != ',' && !posOver) {
                std::cerr << "Error reading file : Positions should be separated by a comma ( , )." << std::endl;
                std::exit(-1);
            }
        }
        std::cout << "Finished reading positions for " << positions.size() << std::endl;

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
        std::cout << "Read background : " << std::endl;
        printBackground();
        std::tie(x, y) = positions[0];
        std::cout << "First base : " << background[y][x];
    }
}

void Battlefield::printBackground() {
    std::for_each(background.begin(), background.end(), [](const string &l) { std::cout << l; });
}
