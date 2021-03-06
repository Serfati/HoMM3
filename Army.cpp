/*
 * Army.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: serfati
 */

#include "Army.h"

using namespace std;

Army::Army() {
    try {
        realArmy = {new Zombie(),new Vampire(),new Archer(),
                    new Wizard(),new BlackDragon()};
        vector<int> vect(6); // emty armyList
        armyList = vect;
        vect.clear();
    }
    catch( std::bad_alloc &exc ) {
        cout << exc.what() << endl;
    }
}

bool Army::isDestroyed() {
    return getArmySize() < 1;
}

bool Army::addUnit(int creatureType,int quantity) {
    armyList[creatureType] += quantity;
    return 1;
}

int Army::getArmySize() {
    int size = 0;
    for (int i : armyList)
        size += i;
    return size;
}

string Army::showArmy() {
    string showArmy = "",t = " \t\r\f\v";
    Creature c;
    for (int i = 5; i >= 0; i --) {
        if ( armyList[i] != 0 ) {
            showArmy += std::to_string(armyList[i]);
            showArmy += " " + c.creaType(i) + " ";
        }
    }
    showArmy.erase(showArmy.find_last_not_of(t) + 1);
    showArmy += ".";
    return showArmy;
}

Army::~Army() {
    for(Creature* i : realArmy)
        delete i;
    armyList.clear();
}

std::string Army::saveArmy() {
    std::string saveArmy = "";
    for (int i = 4; i >= 0; i --) {
        saveArmy += std::to_string(armyList[i]);
        if ( i != 0 )
            saveArmy += " ";
        else
            saveArmy += "";
    }
    return saveArmy;
}

void Army::buildArmy(int BD,int WZ,int ARC,int VMP,int ZMB) {
    armyList[4] = BD;
    armyList[3] = WZ;
    armyList[2] = ARC;
    armyList[1] = VMP;
    armyList[0] = ZMB;
}

void Army::reBuild() {
    for(Creature* i : realArmy)
        delete i;
    realArmy = {new Zombie(),new Vampire(),new Archer(),
                new Wizard(),new BlackDragon()};

}