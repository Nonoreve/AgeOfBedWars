//
// Created by nonoreve on 1/5/21.
//
#include "Battlefield.hpp"
#include "units/Infantryman.hpp"
#include "yaml-cpp/yaml.h"

#include <iostream>
#include <algorithm>
#include <tuple>
#include <chrono>
#include <thread>

/**
 * Extract all battlefield informations from filename.
 * The file should be written as follows :
 * (first line) size: integer, number of cells between the two bases
 * (second line) comma separated list of 2D coordinates
 * (third and more lines) caracter representation of the background of the battlefield
 */
Battlefield::Battlefield(UnitPool &unitPool, const string &filename, int baseHealth) : _unitPool(unitPool) {
	YAML::Node config = YAML::LoadFile(filename);
	if (!config.IsMap() || !config["grid"] || !config["bases"] || !config["background"]) {
		std::cerr << "Configuration of file \"" << filename
		          << "\" is incomplete. Check provided examples for valid yaml structure." << std::endl;
		std::exit(-1);
	}

	// loading grid information
	if (!config["grid"].IsMap() || !config["grid"]["X"] || !config["grid"]["X"].IsScalar()) {
		std::cerr << R"("grid" map must contain at least an "X" scalar.)" << std::endl;
		std::exit(-1);
	}
	bool setup2D = false;
	if (config["grid"]["Y"]) {
		if (!config["grid"]["Y"].IsScalar()) {
			std::cerr << R"("Y" must be a scalar.)" << std::endl;
			std::exit(-1);
		}
		_cellsGrid = std::make_pair(config["grid"]["X"].as<int>(), config["grid"]["Y"].as<int>());
		if (_cellsGrid.second == 1) // Y = 1 -> effectively one-dimensional
			setup2D = true;
	} else {
		_cellsGrid = std::make_pair(config["grid"]["X"].as<int>(), 1);
		setup2D = true;
	}

	// loading bases information
	if (!config["bases"].IsSequence()) {
		std::cerr << R"("bases" sequence must contain a list of positions.)" << std::endl;
		std::exit(-1);
	}
	for (int i = 0; i < config["bases"].size(); i++) {
		if (!config["bases"][i].IsMap() || !(config["bases"][i]["X"])) {
			std::cerr << R"(Positions should contain at least an "X" scalar.)" << std::endl;
			std::exit(-1);
		}
		if (!setup2D && !config["bases"][i]["Y"]) {
			std::cerr << R"("Y" scalar is mandatory if grid is two-dimensional.)" << std::endl;
			std::exit(-1);
		}
		if (setup2D)
			_bases.emplace_back(baseHealth, Position(config["bases"][i]["X"].as<int>(), 0));
		else
			_bases.emplace_back(baseHealth,
			                    Position(config["bases"][i]["X"].as<int>(), config["bases"][i]["Y"].as<int>()));
	}
	// loading background information
	if (!config["background"].IsScalar()) {
		std::cerr << R"("background" scalar should contain the terrain as a list of newline ended strings.)"
		          << std::endl;
		std::exit(-1);
	}
	std::istringstream background(config["background"].as<string>());
	string line;
	while (!background.eof()) {
		getline(background, line);
		line.push_back('\n');
		_background.emplace_back(line.begin(), line.end());
	}


	// doing some treatment and sanity checks

	if (_cellsGrid.first <= 1 || _cellsGrid.second <= 0) {
		std::cerr << "Minimal valid grid size is X=2, Y=1. Given grid size is X=" << _cellsGrid.first << ", Y="
		          << _cellsGrid.second << std::endl;
	}

	_background.at(_background.size() - 1).at(0) = ' '; // removing last LF
	// fill last line with spaces based on the size of the line before
	for (int i = 0; i < _background.at(_background.size() - 2).size(); i++) {
		_background.at(_background.size() - 1).push_back(' ');
	}
	_background.at(_background.size() - 1).push_back('\n');

	std::for_each(_bases.begin(), _bases.end(), [&](const Base &base) {
		Position p = base.getPosition();
		if (p.x < 0 || p.x > _background[0].size() || p.y < 0 || p.y > _background.size()) {
			std::cerr << "Position out of terrain : " << p.x << ", " << p.y << std::endl;
			std::exit(-1);
		}
	});

	std::cout << "Read background : length=" << _background[0].size() << " size=" << _background.size() << std::endl;
	printBackground(_background);
}

int Battlefield::_baseIndex = 0;

Base &Battlefield::getBaseInCreatedOrder(const string &playerName) {
	_bases.at(_baseIndex).setMark((char) std::toupper(playerName.at(0)));
	return _bases.at(_baseIndex++);
}

int Battlefield::basesStatus() {
	bool destroyedBaseFound = false;
	auto currentBase = _bases.begin();
	while (currentBase != _bases.end() && !destroyedBaseFound) {
		destroyedBaseFound = !currentBase->stillAlive();
		++currentBase;
	}
	return destroyedBaseFound ? (int) std::distance(_bases.begin(), currentBase) - 1 : -1;
}

void Battlefield::playActions(Player &currentPlayer) {
	vector<Position> playerPos = _unitPool.getHisArmy(currentPlayer);

	std::cout << "Phase 1 for " << currentPlayer.getName() << std::endl;
	std::chrono::milliseconds skipTime(600);
	std::this_thread::sleep_for(skipTime);
	for (int i = (int) playerPos.size() - 1; i >= 0; i--) {
		doActionPhase(1, _unitPool.getUnit(playerPos.at(i)));
	}
	drawTerrain();

	std::cout << "Phase 2 for " << currentPlayer.getName() << std::endl;
	std::this_thread::sleep_for(skipTime);
	playerPos = _unitPool.getHisArmy(currentPlayer);
	for (int i = 0; i < playerPos.size(); i++) {
		doActionPhase(2, _unitPool.getUnit(playerPos.at(i)));
	}
	drawTerrain();

	std::cout << "Phase 3 for " << currentPlayer.getName() << std::endl;
	std::this_thread::sleep_for(skipTime);
	playerPos = _unitPool.getHisArmy(currentPlayer);
	for (int i = 0; i < playerPos.size(); i++) {
		doActionPhase(3, _unitPool.getUnit(playerPos.at(i)));
	}
	drawTerrain();
}

void Battlefield::doActionPhase(int actionPhase, Unit *unit) {
	switch (unit->getAction(actionPhase)) { // TODO the whole process might be cleaned up
		case MOVE:
			if (unit->targetReached())
				break; // stop unit before target cell
			if (_unitPool.move(unit->nextWantedPosition(), unit->getPosition()))
				unit->move();
			break;
		case ATTACK: {
			std::pair result = unit->attack(_unitPool.getHisEnnemies(unit));
			for (auto p : result.first) {
				auto type = hitThere(p, result.second);
				if (type != UnitType::UNKNOWN)
					unit->rewardOwner(type);
				if (type == UnitType::INFANTRYMAN && unit->getType() == INFANTRYMAN) {
					auto *soldier = static_cast<Infantryman *>(unit); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
					soldier->upgrade();
				}
			}
			// hit bases
			bool baseFound = false;
			auto it = _bases.begin();
			while (!baseFound && it != _bases.end()) {
				if (unit->getTargetPostion() == it->getPosition()) {
					baseFound = true;
					if (unit->targetReached() && _unitPool.isCellFree(it->getPosition())) {
						it->takeDamage(result.second);
						_unitPool.remove(unit->getPosition());
					}
				}
				++it;
			}
			buryCorpses(); // done after base damages to avoid hitting unit and base at the same time
		}
			break;
		case IDLE: // do nothing
			break;
	}
}

UnitType Battlefield::hitThere(Position position, int damages) {
	if (!_unitPool.isCellFree(position)) {
		_unitPool.getUnit(position)->takeDamages(damages);
		if (!_unitPool.getUnit(position)->stillAlive()) {
			return _unitPool.getUnit(position)->getType();
		}
	}
	return UnitType::UNKNOWN;
}

void Battlefield::buryCorpses() {
	for (Position position : _unitPool.getAllPositions())
		if (!_unitPool.getUnit(position)->stillAlive()) {
			_unitPool.remove(position);
		}
}

void Battlefield::drawTerrain() {
	std::cout << "Current state of terrain : " << std::endl;
	vector<vector<char>> frame = _background;
	float charPerCellX = (float) (frame[0].size() - 1) / (float) _cellsGrid.first; // -1 for linefeed
	float charPerCellY = (float) (frame.size() -1 ) / (float) _cellsGrid.second; // -1 to make it works
	int offsetX = 3; // TODO offsets
	int offsetY = 2;
	// draw bases marks and health
	for (auto &base : _bases) {
		editTerrainAt(frame, (int) std::floor(charPerCellX * (float) base.getPosition().x) + offsetX,
		              (int) std::floor(charPerCellY * (float) base.getPosition().y) + offsetY, base.getMark());
		string health = std::to_string(base.getHealth());
		for (int i = 0; i < health.size(); i++) {
			int cOffset = offsetX - health.size() / 2 + i;
			editTerrainAt(frame, (int) std::floor(charPerCellX * (float) base.getPosition().x) + cOffset,
			              (int) std::floor(charPerCellY * (float) base.getPosition().y) + offsetY + 2, health.at(i));
		}
	}
	// draw units
	for (auto &pos : _unitPool.getAllPositions()) {
		string label = _unitPool.getUnit(pos)->getLabel();
		for (int i = 0; i < label.size(); i++) {
			int cOffset = offsetX - label.size() / 2 + i;
			editTerrainAt(frame, (int) std::floor(charPerCellX * (float) pos.x) + cOffset,
			              (int) std::floor(charPerCellY * (float) pos.y) + offsetY, label.at(i));
		}
		//		editTerrainAt(frame, (int) std::round(charPerCellX * (float) pos.x) + offsetX,
		//		              (int) std::round(charPerCellY * (float) pos.y) + offsetY, _unitPool.getUnit(pos)->getMark());
		string health = std::to_string(_unitPool.getUnit(pos)->getHealth());
		for (int i = 0; i < health.size(); i++) {
			int cOffset = offsetX - health.size() / 2 + i;
			editTerrainAt(frame, (int) std::floor(charPerCellX * (float) pos.x) + cOffset,
			              (int) std::floor(charPerCellY * (float) pos.y) + offsetY + 1, health.at(i));
		}
	}
	printBackground(frame);
}

void Battlefield::printBackground(vector<vector<char>> &terrain) {
	std::for_each(terrain.begin(), terrain.end(), [](const vector<char> &l) {
		std::for_each(l.begin(), l.end(), [](const char c) { std::cout << c; });
	});
}

void Battlefield::editTerrainAt(vector<vector<char>> &terrain, int posX, int posY, char tileToPut) {
	terrain[posY][posX] = tileToPut;
}
