//
// Created by nonoreve on 1/5/21.
//

#include "Player.hpp"

#include <utility>
#include <iostream>

Player::Player(string name, int initialMoneyAmount, Base base) : _name(std::move(name)), _money(initialMoneyAmount),
                                                                 _base(std::move(base)) {
}

void Player::pay(int pay) {
	_money += pay;
}

string Player::report() const {
	return "Money : " + std::to_string(_money);
}

bool Player::canAfford(int price) {
	return price <= _money;
}

void Player::buy(UnitType unitType) {
	if (unitType != UNKNOWN && canAfford(Unit::UNIT_PRICES[unitType]))
		_money -= Unit::UNIT_PRICES[unitType];
	else{
		std::cerr << "Can't afford unit" << std::endl;
		exit(-1);
	}
}
