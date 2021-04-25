//
// Created by nonoreve on 1/5/21.
//

#include "Player.hpp"

#include <utility>
#include <iostream>
#include <algorithm>

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

UnitType Player::chooseUnit() const {
	if (isAi()) {}
	else {
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
