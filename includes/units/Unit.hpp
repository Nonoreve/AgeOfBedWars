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
	const string _label;

	Unit(int health, int strikePower, Player &player, Base &target, const string &label);

public:
	virtual ActionType getAction(int actionPhase) = 0;

	/**
	 * @param ennemies The positions of the ennemies that are on the same line as this unit.
	 * (Ennemies that targets the Base of the owner of this Unit)
	 * @return All teh cell positions that the unit strikes
	 */
	virtual std::pair<vector<Position>, int> attack(vector<Position> ennemies) = 0;

	virtual Position nextWantedPosition();

	virtual void move();

	virtual void takeDamages(int damages);

	virtual bool stillAlive() const;

	virtual bool targetReached();

	virtual bool belongsTo(Player &player) const;

	virtual bool sameOwner(Unit *other) const;

	const string & getLabel() const {
		return _label;
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
