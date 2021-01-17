//
// Created by nonoreve on 1/5/21.
//

#include "Player.hpp"

#include <utility>

Player::Player(string name, int initialMoneyAmount, Base base) : name(std::move(name)),
                                                                 money(initialMoneyAmount),
                                                                 base(base) {}

void Player::play() {

}

const string &Player::getName() const {
    return name;
}

void Player::pay(int pay) {
    money += pay;
}

bool Player::summon(string unitType, Base target) {
    if (unitType == "infantry") {
        Infantryman(base.getPosition());
        return true;
    } else if(unitType == "archer"){

    } else if(unitType == "catapult"){

    }
    return false;
}

string Player::report() {
    return name + " : \nMoney : " + std::to_string(money);
}

bool Player::canSummon() {
    return base.canSummon();
}

int Player::getMoney() const {
    return money;
}
