/*
 * Necromancer.h
 *
 *  Created on: Dec 24, 2018
 *      Author: serfati
 */
#pragma once
#ifndef NECOMANCER_H_
#define NECOMANCER_H_

#include "Hero.h"
#include "Creature.h"
#include "Zombie.h"

using namespace std;

class Necromancer : public Hero {

public:

    Necromancer() { setType(necromancer); }

    Necromancer(string);

    virtual bool specialAbility(Hero &);

    ~Necromancer();

};

#endif /* NECOMANCER_H_ */
