//
// Created by nonoreve on 1/5/21.
//

#include "Player.hpp"

#include <utility>
#include <iostream>
#include <algorithm>
#include <random>

Player::Player(string name, int initialMoneyAmount, Base base, bool ai) : _name(std::move(name)),
                                                                          _money(initialMoneyAmount),
                                                                          _base(std::move(base)), _ai(ai) {
}

void Player::pay(int pay) {
	_money += pay;
}

string Player::report() const {
	return "You have " + std::to_string(_money) + " money left.";
}

bool Player::canAfford(int price) const {
	return price <= _money;
}

void Player::buy(UnitType unitType) {
	if (unitType != UNKNOWN && canAfford(Player::UNIT_PRICES[unitType]))
		_money -= Player::UNIT_PRICES[unitType];
	else {
		std::cerr << "Can't afford unit" << std::endl;
		exit(-1);
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

// TODO needs refactoring (change UNIT_PRICES to an action enum after unit templatism)
UnitType Player::chooseUnit() const {
	if (isAi()) {
		UnitType choice = UNKNOWN;
		vector<int> validUnits; // stores units indices the player can afford, from cheapest to priciest
		validUnits.emplace_back(3); // push option for to not buy unit, pointing to UNIT_PRICES::UNKNOWN
		for (int i = 0; i < 3; i++) {
			if (canAfford(UNIT_PRICES[i]))
				validUnits.emplace_back(i);
		}
		if (validUnits.size() > 1) {
			int rng = std::rand() % 100;
//			std::cout << "DEBUG RNG : " << rng << std::endl;
			if (rng > 80) {
				std::cout << "Random play" << std::endl;
				// choose a random valid action
				vector<int> selected;
				std::sample(validUnits.begin(), validUnits.end(), std::inserter(selected, selected.begin()), 1,
				            std::default_random_engine{std::random_device{}()});
				// we can map directly because UNIT_PRICES and UnitType are in the same order
				choice = static_cast<UnitType>(selected.front());
			} else {
				// choose the most expensive unit available
				std::cout << "Optimal play" << std::endl;
				choice = static_cast<UnitType>(validUnits.back());
			}
		} else if (!validUnits.empty()) {
			// only one option available
			choice = static_cast<UnitType>(validUnits.front());
		}
		switch (choice) {
			case 0:
				std::cout << getName() << " summoned an infantry." << std::endl;
				break;
			case 1:
				std::cout << getName() << " summoned an archer." << std::endl;
				break;
			case 2:
				std::cout << getName() << " summoned a catapult." << std::endl;
				break;
			default:
				std::cout << getName() << " did nothing." << std::endl;
		}
		return choice;
	} else {
		bool validForm = false;
		while (!validForm) {
			string unit;
			std::cout << "Do you want to summon a new unit ? "
			             "(infantry|archer|catapult|none)" << std::endl;
			std::cin >> unit;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush stream
			UnitType unitType = parseUnitName(unit);
			if (unitType != UNKNOWN)
				return unitType;
			if (unit == "none") {
				std::cout << "No unit summoned." << std::endl;
				validForm = true;
			} else {
				std::cout << "Wrong unit name : " << unit << ". Try again." << std::endl;
			}
		}
	}
	return UNKNOWN;
}

bool Player::operator==(const Player &rhs) const {
	return _name == rhs._name && _base == rhs._base && _ai == rhs._ai;
}
