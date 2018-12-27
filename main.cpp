/*
 ============================================================================
 Name        : Heroes of Might and Magic® 3
 Author      : Avihai Serfati (204520803)
 Version     : 1.1
 Copyright   : Read Only
 Dialect     : C++
 Description : implements Heroes of Might and Magic 3 a turn-based
  	  	  	   strategy game developed by Jon Van Caneghem in 1999-2000
 ============================================================================
 */


//#include <iostream>
//#include <string>
//#include <vector>
//#include "Army.h"
//
//#include "Creature.h"
//#include "Vampire.h"
//#include "Archer.h"
//#include "BlackDragon.h"
//#include "Zombie.h"
//#include "Wizard.h"
//
//#include "Hero.h"
//#include "Thief.h"
//#include "Warrior.h"
//#include "Necomancer.h"
//#include <iostream>
//#include <stdio.h>
//#include <sys/stat.h>
#include "GameUnit.h"

using namespace std;

/* this is the main routine where all the glue logic fits     */

/***************************************/
/* 	  Heroes of Might and Magic® 3	  */
/***************************************/

int main(int argc , char* argv[]){

    if( argv[2] ) { /*    New Game   */
        /* - [ Arguments ] -    argv[0] is the name of the program; Heroes  */
        const string playOrLoad = argv[1];
        const int warriors = atoi(argv[2]);
        const int thieves = atoi(argv[3]);
        const int Necromancers = atoi(argv[4]);
        GameUnit gamePlay = GameUnit(warriors, thieves, Necromancers);
    }
    else        /*  Load last Game  */
        GameUnit gamePlay;

    cout<<"***END OF MAIN***‬‬"<<endl;

}