//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_UNIT_HPP
#define AGEOFBEDWARS_UNIT_HPP

#include "UnitType.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "Base.hpp"

class Unit {
private:
	int _health;
	Player _owner;

protected:
	Base _target;
	Position _position;
	int _strikePower;

	Unit(int health, int strikePower, Player &player, Base &target);

public:
	virtual ActionType getAction(int actionPhase) = 0;

	virtual std::pair<Position, int> attack() = 0;

	virtual Position &move();

	virtual void takeDamages(int damages);

	virtual bool stillAlive() const;

	virtual bool targetReached();

	virtual bool belongsTo(Player &player) const;

	inline Position getPosition() const {
		return _position;
	}

	int getHealth() const {
		return _health;
	}
};


#endif //AGEOFBEDWARS_UNIT_HPP
