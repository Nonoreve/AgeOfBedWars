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
	const char _mark;

	Unit(int health, int strikePower, Player &player, Base &target, char mark);

public:
	virtual ActionType getAction(int actionPhase) = 0;

	/**
	 * @param ennemies The positions of the ennemies that are on the same line as this unit.
	 * (Ennemies that targets the Base of the owner of this Unit)
	 * @return All teh cell positions that the unit strikes
	 */
	virtual std::pair<vector<Position>, int> attack(vector<Position> ennemies) = 0;

	virtual Position nextWantedPotition();

	virtual void move();

	virtual void takeDamages(int damages);

	virtual bool stillAlive() const;

	virtual bool targetReached();

	virtual bool belongsTo(Player &player) const;

	virtual bool sameOwner(Unit *other) const;

	char getMark() const {
		return _mark;
	}

	inline Position getPosition() const {
		return _position;
	}

	int getHealth() const {
		return _health;
	}

	const Position &getTargetPostion() const {
		return _target.getPosition();
	}
};


#endif //AGEOFBEDWARS_UNIT_HPP
