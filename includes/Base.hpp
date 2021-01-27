//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_BASE_HPP
#define AGEOFBEDWARS_BASE_HPP


#include "Position.hpp"

class Base {
private:
    int _health;
    // position in cell coordinates
    const Position _position;

public:
    explicit Base(int health, Position position);

	void takeDamage(int damages);

    const Position &getPosition() const;

	inline int getHealth() const{
		return _health;
	}
};


#endif //AGEOFBEDWARS_BASE_HPP
