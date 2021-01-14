//
// Created by nonoreve on 1/5/21.
//

#include "Player.hpp"

#include <utility>

Player::Player(string name, int initialMoneyAmount, Base base) : name(std::move(name)),
                                                                 money(initialMoneyAmount),
                                                                 base(base) {}
