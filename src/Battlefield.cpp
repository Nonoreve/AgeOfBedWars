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
Battlefield::Battlefield(UnitPool &unitPool, const string &filename, int baseHealth) : _unitPool(unitPool) {
	std::ifstream istrm(filename, std::ios::binary);
	if (!istrm.is_open()) {
		std::cerr << "Failed to open " << filename << std::endl;
	} else {
		int sizeX = 1, sizeY = 1; // TODO read sizeY (upgrade parser to YAML)
		if (!(istrm >> sizeX)) {
			std::cerr << "Error reading file : Background files should start by the size of the grid." << std::endl;
			std::exit(-1);
		}
		std::cout << "Size of the grid : " << sizeX << ", " << sizeY << std::endl;
		_cellsGrid = std::make_pair(sizeX, sizeY);
		istrm.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


		string line;
		std::getline(istrm, line);
		std::istringstream iss(line);
		std::cout << "Pos line : " << line << std::endl;
		int x, y;
		bool posOver = false;
		while (!posOver && iss >> x >> y) {
			std::cout << "New position : x=" << x << " y=" << y << std::endl;
			_bases.emplace_back(baseHealth, Position(x, y));
			char comma = '\0'; // TODO all the positions should be different
			iss >> comma; // trying to read a comma
			posOver = comma == '\0'; // if nothing is read we reached the end of the line
			if (comma != ',' && !posOver) {
				std::cerr << "Error reading file : Positions should be separated by a comma ( , )." << std::endl;
				std::exit(-1);
			}
		}

		std::cout << "Finished reading " << _bases.size() << " bases Positions." << std::endl;
		// TODO unit sprites
		while (!istrm.eof()) {
			getline(istrm, line);
			line.push_back('\n');
			_background.emplace_back(line.begin(), line.end());
		}
		if (_background.empty()) {
			std::cerr << "Error reading file : Background files should contain at least a string at line 3."
			          << std::endl;
			std::exit(-1);
		}
		std::for_each(_bases.begin(), _bases.end(), [&](const Base &base) {
			Position p = base.getPosition();
			if (p.x < 0 || p.x > _background[0].size() || p.y < 0 || p.y > _background.size()) {
				std::cerr << "Position out of terrain : " << p.x << ", " << p.y << std::endl;
				std::exit(-1);
			}
		});

		std::cout << "Read background : length=" << _background[0].size() << " size=" << _background.size()
		          << std::endl;
		printBackground(_background);
	}
};

int Battlefield::_baseIndex = 0;

Base &Battlefield::getBaseInCreatedOrder(const string &playerName) {
	_bases.at(_baseIndex).setMark(std::toupper(playerName.at(0)));
	return _bases.at(_baseIndex++);
}

int Battlefield::basesStatus() {
	bool destroyedBaseFound = false;
	auto currentBase = _bases.begin();
	while (currentBase != _bases.end() && !destroyedBaseFound) {
		destroyedBaseFound = !currentBase->stillAlive();
		++currentBase;
	}
	return destroyedBaseFound ? (int) std::distance(_bases.begin(), currentBase) : -1;
}

void Battlefield::playActions() {
	std::vector<Position> allPos = _unitPool.getAllPositions();
	// phase 1
	for (int i = (int) allPos.size() - 1; i >= 0; i--) {
		doActionPhase(1, _unitPool.getUnit(allPos.at(i)));
	}
	allPos = _unitPool.getAllPositions();
	// phase 2
	for (int i = 0; i < allPos.size(); i++) {
		doActionPhase(2, _unitPool.getUnit(allPos.at(i)));
	}
	allPos = _unitPool.getAllPositions();
	// phase 3
	for (int i = 0; i < allPos.size(); i++) {
		doActionPhase(3, _unitPool.getUnit(allPos.at(i)));
	}
}

void Battlefield::doActionPhase(int actionPhase, Unit *unit) {
	switch (unit->getAction(actionPhase)) {
		case MOVE:
			// destination is rightmost because we want to apply the move after getting its previous value
			_unitPool.move(unit->move(), unit->getPosition());
			break;
		case ATTACK: {
			std::pair result = unit->attack();
			hitThere(result.first, result.second);
			// hit bases
			bool baseFound = false;
			auto it = _bases.begin();
			while (!baseFound && it != _bases.end()) {
				if (it->getPosition() == unit->getPosition()) {
					it->takeDamage(result.second);
					_unitPool.remove(unit->getPosition());
					baseFound = true;
				}
				++it;
			}
		}
			break;
		case IDLE: // do nothing
			break;
	}
}

void Battlefield::hitThere(Position position, int damages) {
	if (!_unitPool.isCellFree(position)) {
		_unitPool.getUnit(position)->takeDamages(damages);
		if (!_unitPool.getUnit(position)->stillAlive()) {
			// TODO player reward
			_unitPool.remove(position);
		}
	}
}

void Battlefield::drawTerrain() {
	std::cout << "Current state of terrain : " << std::endl;
	vector<vector<char>> frame = _background;
	float charPerCellX = (float) (frame[0].size() - 1) / (float) _cellsGrid.first; // -1 for linefeed
	float charPerCellY = (float) frame.size() / (float) _cellsGrid.second;
	int offsetX = 3; // TODO offsets
	int offsetY = 2;
	// draw bases marks
	for (auto &base : _bases) {
		editTerrainAt(frame, (int) std::round(charPerCellX * (float) base.getPosition().x) + offsetX,
		              (int) std::round(charPerCellY * (float) base.getPosition().y) + offsetY, base.getMark());
	}
	// draw units
	for (auto &pos : _unitPool.getAllPositions()) {
		editTerrainAt(frame, (int) std::round(charPerCellX * (float) pos.x) + offsetX,
		              (int) std::round(charPerCellY * (float) pos.y) + offsetY, 'X');
	}
	printBackground(frame);
	// TODO print health of unit or base under each case
}

void Battlefield::printBackground(vector<vector<char>> &terrain) {
	std::for_each(terrain.begin(), terrain.end(), [](const vector<char> &l) {
		std::for_each(l.begin(), l.end(), [](const char c) { std::cout << c; });
	});
}

void Battlefield::editTerrainAt(vector<vector<char>> &terrain, int posX, int posY, char tileToPut) {
	terrain[posY][posX] = tileToPut;
}
