//
// Created by nonoreve on 1/5/21.
//

#include "Player.hpp"

#include <utility>

Player::Player(string name, int initialMoneyAmount, Base base) : _name(std::move(name)), _money(initialMoneyAmount),
                                                                 _base(std::move(base)) {
}

void Player::pay(int pay) {
	_money += pay;
}

string Player::report() const {
	return _name + " : \nMoney : " + std::to_string(_money);
}
