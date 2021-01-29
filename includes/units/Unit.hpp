//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_UNIT_HPP
#define AGEOFBEDWARS_UNIT_HPP

#include "Position.hpp"
#include "Base.hpp"

enum ActionType {
	MOVE, ATTACK, IDLE
};

enum UnitType {
	INFANTRYMAN, ARCHER, CATAPULT, UNKNOWN

};


class Unit {
private:
	int _health;

protected:
	Unit(int health, int strikePower, Position position, Base target);

	Base _target;
	Position _position;
	int _strikePower;

public:
	virtual ActionType getAction(int actionPhase) = 0;

	virtual std::pair<Position, int> attack() = 0;

	virtual Position& move();

	virtual void takeDamages(int damages);

	virtual bool stillAlive() const;

	inline virtual const Position getPosition() const {
		return _position;
	}

	constexpr static int UNIT_PRICES[] = {10, 12, 20};
};


#endif //AGEOFBEDWARS_UNIT_HPP
