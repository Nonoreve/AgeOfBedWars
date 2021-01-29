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
    char _mark;

public:
    explicit Base(int health, Position position);

	void takeDamage(int damages);

    const Position &getPosition() const;

	bool stillAlive() const;

	int getHealth() const {
		return _health;
	}

	char getMark() const {
		return _mark;
	}

	void setMark(char mark) {
		_mark = mark;
	}
};


#endif //AGEOFBEDWARS_BASE_HPP
