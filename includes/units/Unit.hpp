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

	virtual void move();

	virtual void takeDamages(int damages);

	virtual bool stillAlive() const;

	inline virtual const Position &getPosition() const {
		return _position;
	}
	// TODO static unit prices
};


#endif //AGEOFBEDWARS_UNIT_HPP
