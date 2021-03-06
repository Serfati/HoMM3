//
// Created by serfati on 12/27/18.
//
#include "GameUnit.h"
#include <algorithm>

using namespace std;

//^^^^^^^^^^ STATIC VARs ^^^^^^^^^^//
static int numberOfPlayers = 0;
static int roundNumber = 0;

//^^^^^^^^^^ Constructors and Destructor ^^^^^^^^^^//
GameUnit::GameUnit() {
    // First Level
    string path = "game/Game_Details.txt";
    const char *runComand = path.c_str();
    ifstream in;
    in.open(runComand);
    if ( ! in )
        cout << "GD: Error reading from file..." << endl;
    string cleanHeader;
    getline(in,cleanHeader,';');
    in >> GameUnit::currentTurn >> ::roundNumber >> ::numberOfPlayers;
    in.close();

    // Second Level

    path = "game/Real_Order.txt";
    const char *runComand2 = path.c_str();
    ifstream ina;
    vector<string> HeroesNames;
    ina.open(runComand2);
    if ( ! ina )
        cout << "RO: Error reading from file..." << endl;
    getline(ina,cleanHeader,';');
    string word;
    while ( ina >> word )
        HeroesNames.push_back(word);
    ina.close();
    load(HeroesNames);
    turnOrder = realOrder;
    shuffle();
    mainMenu(turnOrder[currentTurn]);
}

GameUnit::GameUnit(const int w,const int t,const int n) {
    try {
        if ( w < 0 || w > 3 || t < 0 || t > 3 || n < 0 || n > 3 )
            throw (std::invalid_argument("Error: invalid number of heroes!"));
    } catch( std::invalid_argument &exc ) {
        cout << exc.what() << endl;
        return;
    }

    rmdir();
    string _name;
    try {
        /*      warriors     */
        do {
            for (int i = 0; i < w; ++ i) {
                try {
                    std::cout << "Please insert warrior number " << i + 1 << " name:" << endl;
                    getline(cin,_name);
                    realOrder.push_back(new Warrior(_name));
                } catch( std::invalid_argument &exc ) {
                    cout << exc.what() << endl;
                    i --;
                }
            }
        } while ( _name.length() > 31 );
        /*      thieves     */
        do {
            for (int i = 0; i < t; ++ i) {
                try {
                    std::cout << "Please insert thief name number " << i + 1 << " name:" << endl;
                    getline(cin,_name);
                    realOrder.push_back(new Thief(_name));
                } catch( std::invalid_argument &exc ) {
                    cout << exc.what() << endl;
                    i --;
                }
            }
        } while ( _name.length() > 31 );
        /*      necromancers     */
        do {
            for (int i = 0; i < n; ++ i) {
                try {
                    std::cout << "Please insert necromancer number " << i + 1 << " name:" << endl;
                    getline(cin,_name);
                    realOrder.push_back(new Necromancer(_name));
                } catch( std::invalid_argument &exc ) {
                    cout << exc.what() << endl;
                    i --;
                }
            }
        } while ( _name.length() > 31 );

    } catch( std::bad_alloc &exc ) {
        cout << exc.what() << endl;
        ::numberOfPlayers --;
    } catch( std::invalid_argument &exc ) {
        cout << exc.what() << endl;
    }
    ::numberOfPlayers = w + t + n;
    ::roundNumber ++;
    turnOrder = realOrder; /*  just a pointer to a shuffled realOrder    */
    this->currentTurn = 0;
    mkdir();
    shuffle();
    save();
    mainMenu(turnOrder[currentTurn]);
}

GameUnit::~GameUnit() {
//    for (Hero *i :realOrder)
//        delete i;
    for (int i = 0; i <realOrder.size(); i ++)
        delete realOrder[i];
    turnOrder.clear();
}

//   ******** Game Main Menu *******    //
bool GameUnit::mainMenu(Hero *turn) {
    if ( turn->inLife()) {
        bool isTurnRun = 1;
        string index;
        string toRob;
        Creature c;
        Hero *ptr = NULL;
        cout << "Welcome " << turn->getName() << endl;
        while ( isTurnRun ) {
            cout << "What is your next step in the path to victory?" << endl;
            cout << "‫‪1. Attack‬‬" << endl;
            cout << "2. Get daily gold" << endl;
            cout << "‫‪3.‬‬ Buy creatures‬‬" << endl;
            cout << "4. Show details" << endl;
            cout << "‫‪5‬‬.‬‬ Special skill‬‬‬‬" << endl;
            cout << "6. End of my turn" << endl;
            cout << "7.‬ Exit‬‬‬‬" << endl;
            cin >> index;

            int choice = atoi(index.c_str());
            switch ( choice ) {
                case 1:    /*	Attack‬‬	*/
                    if ( ::roundNumber > 3 )
                        attackMenu(turn);
                    else cout << "Can'nt attack before round 4, You're still on round " << ::roundNumber << endl;
                    save();
                    if ( ::numberOfPlayers <= 1 ) { // last player
                        cout << realOrder[0]->getName() + " is the winner!" << endl;
                        rmdir();
                        return (EXIT_SUCCESS);
                    }
                    break;

                case 2:    /*	Get daily gold	*/
                    if ( ! turn->isGotDG()) {
                        turn->getDailyGold();
                        turn->setGotDG(true);
                    } else cout << "You already got your daily gold!" << endl;
                    break;

                case 3: /*	Buy creatures	*/
                    try {
                        storeMenu(turn);
                    } catch( std::invalid_argument &e ) { cout << e.what() << endl; }
                    break;

                case 4:    /*	Show details	*/
                    turn->showHero();
                    break;

                case 5:    /*	Special skill‬‬‬‬	*/
                    if ( ! turn->isUsedSA()) {
                        try {
                            if ( turn->getType() == thief ) {
                                std::cout << "Please insert hero name:" << endl;
                                cin >> toRob;
                                ptr = getHeroByName(toRob);
                                if ( NULL == ptr )
                                    throw std::invalid_argument("Hero to rob not found!");
                                turn->specialAbility(*ptr);
                                turn->setUsedSA(true);
                                break;
                            }
                        } catch( std::invalid_argument &e ) { cout << e.what() << endl; }
                        turn->specialAbility(*ptr);
                        turn->setUsedSA(true);
                    } else cout << "You already used your special ability! wait for next round." << endl;
                    break;

                case 6:    /*	End of my turn	*/
                    turn->setUsedSA(false);
                    turn->setGotDG(false);
                    save();
                    isTurnRun = false;
                    nextTurn();
                    break;

                case 7: /* 	‫‪Exit‬‬‬‬  */
                    save();
                    return 1;

                default:
                    cout << "please choose a valid number" << endl;
                    break;
            }
        }
    }
    return false;
}

bool GameUnit::attackMenu(Hero *me) {
    Hero *ptr = NULL;
    string heroToAttack = "",index;
    while ( true ) {
        cout << "‫‪1. Show me my opponents" << endl;
        cout << "2. Attack Hero";
        std::cin.ignore();
        if ( std::cin.peek() != '\n' )
            cin >> index;
        else {
            std::cin.ignore();
            return 0;
        }
        int choice = atoi(index.c_str());
        switch ( choice ) {
            case 1:    /*	Show me my opponents	*/
                showHeroes();
                return 1;
            case 2:    /*  Attack Hero by name	   */
                try {
                    cout << "Please insert your opponent name:" << endl;
                    std::cin.ignore();
                    if ( std::cin.peek() != '\n' )
                        cin >> heroToAttack;
                    ptr = getHeroByName(heroToAttack);
                    if ( NULL == ptr )
                        throw std::invalid_argument("Hero not found!");

                } catch( std::invalid_argument &e ) {
                    cout << e.what() << endl;
                    return 1;
                }
                me->showHeroFight();
                ptr->showHeroFight();
                me->attackEnemy(*ptr);
                eraseKilled();
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}

int GameUnit::storeMenu(Hero *me) {
    Creature c;
    string index;
    while ( 1 > 0 ) {
        int choosenType = 0,count = 0,unitPrice = 0;
        cout << "‫‪1. Buy Zombies." << endl;
        cout << "2. Buy Archers." << endl;
        cout << "‫‪3.‬‬ Buy Vampire." << endl;
        cout << "4. Buy Wizard." << endl;
        cout << "‫‪5‬‬.‬‬ Buy Black Dragon." << endl;
        cin >> index;
        int choice = atoi(index.c_str());
        switch ( choice ) {
            case 1:    /*	Buy Zombies 	*/
                choosenType = 0;
                c = Zombie();
                unitPrice = c.getPrice();
                c.showCreature();
                break;
            case 2:    /*  Buy	Archers	   */
                choosenType = 1;
                c = Archer();
                unitPrice = c.getPrice();
                c.showCreature();
                break;
            case 3:     /*	Buy Vampire	   */
                choosenType = 2;
                c = Vampire();
                unitPrice = c.getPrice();
                c.showCreature();
                break;
            case 4:    /*	Buy Wizard	   */
                choosenType = 3;
                c = Wizard();
                unitPrice = c.getPrice();
                c.showCreature();
                break;
            case 5:    /*	Buy Black Dragon  */
                choosenType = 4;
                c = BlackDragon();
                unitPrice = c.getPrice();
                c.showCreature();
                break;
            default:
                cout << "please choose a valid number" << endl;
        }
        cin >> count;
        if ( unitPrice * count > me->getGold()) {
            throw std::invalid_argument("Not enough gold!");
            return 0;
        }
        me->buyCreature(me->getGold(),choosenType,count,unitPrice);
        return 1;
    }
}

void GameUnit::nextTurn() {
    int check = currentTurn;
    if ( check + 1 >= ::numberOfPlayers ) {
        currentTurn = 0;
        ::roundNumber ++;
        mainMenu(turnOrder[currentTurn]);
    } else {
        currentTurn ++;
        mainMenu(turnOrder[currentTurn]);
    }
}

void GameUnit::eraseKilled() {
    unsigned long size = realOrder.size();
    for (int i = 0; i < size; i ++) {
        if ( realOrder[i]->inLife() == 0 && realOrder[i]->showArmy() == "." ) {
            realOrder[i]->rmdir();
            realOrder[i]->~Hero();
            realOrder.erase(realOrder.begin() + i);
            ::numberOfPlayers --;
        }
    }
}

//^^^^^^^^^^^^^^^^ MAINTENANCE ^^^^^^^^^^^^^^^^^//
void GameUnit::save() {
    string HeroesNames = "";
    for (int i = 0; i < ::numberOfPlayers; ++ i)
        realOrder[i]->save();
    ofstream out;
    out.open("game/Game_Details.txt");
    out << "=[ Heroes Names ]= | TURN | Round# | #OP;" << endl;
    out << currentTurn << " " << ::roundNumber << " " << ::numberOfPlayers << endl;
    out << getTurnOrder();

    for (int i = 0; i < ::numberOfPlayers; ++ i)
        HeroesNames += realOrder[i]->getName() + " ";
    ofstream out2;
    out2.open("game/Real_Order.txt");
    out2 << "=[ Heroes Names ]=;" << endl;
    out2 << HeroesNames;

    out2.close();
    out.close();
}

void GameUnit::load(vector<string> _heroes) {
    // for (std::string i : _heroes)
    for (int i = 0; i < _heroes.size(); i ++) {
        Hero s;
        Warrior *s1;
        Thief *s2;
        Necromancer *s3;
        switch ( s.typeFromFile(_heroes[i])) {
            case 0:
                s1 = new(nothrow) Warrior();
                if ( NULL == s1 ) cout << "Error: memory could not be allocated" << endl;
                s1->load(_heroes[i]);
                realOrder.push_back(s1);
                break;
            case 1:
                s2 = new(nothrow) Thief();
                if ( NULL == s2 ) cout << "Error: memory could not be allocated" << endl;
                s2->load(_heroes[i]);
                realOrder.push_back(s2);
                break;
            case 2:
                s3 = new(nothrow) Necromancer();
                if ( NULL == s3 ) cout << "Error: memory could not be allocated" << endl;
                s3->load(_heroes[i]);
                realOrder.push_back(s3);
                break;
            default:
                cout << "Load failed!";
        }
    }
}

bool GameUnit::mkdir() {
    try {
        string comand = "mkdir game";
        const char *runComand = comand.c_str();
        system(runComand);
    } catch( std::exception &e ) {
        cout << "Error creating directory!" << endl;
        return 0;
    }
    return 1;
}

void GameUnit::shuffle() {
    std::random_shuffle(turnOrder.begin(),turnOrder.end());
}

void GameUnit::rmdir() {
    try {
        string comand = "find * -type d | grep -v \"^CMakeFiles\" | xargs rm -rf";
        const char *runComand = comand.c_str();
        system(runComand);
    } catch( std::exception &e ) {
        cout << "Error deleting directory!n" << endl;
    }
}

//^^^^^^^^^^^^^^ Getters and Setters ^^^^^^^^^^^^^^//
string GameUnit::getTurnOrder() {
    string turnList = "";
//    for (Hero *i : turnOrder)
    for (int i = 0; i <turnOrder.size(); i ++)
        turnList += turnOrder[i]->getName() + " ";
    return turnList;
}

Hero *GameUnit::getHeroByName(string heroName) {
    for (int i = 0; i < ::numberOfPlayers; ++ i)
        if ( realOrder[i]->getName() == heroName )
            return realOrder[i];
    return NULL;
}

void GameUnit::showHeroes() {
    for (int i = 0; i < ::numberOfPlayers; ++ i) {
        if ( realOrder[i]->getName() != turnOrder[currentTurn]->getName()) {
            cout << realOrder[i]->getName() << " ";
            cout << realOrder[i]->displayType() << endl;
        }
    }
}