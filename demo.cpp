// demo.cpp
//
// This program simulates a complete game of Coup with six players—one of each role.
// It exercises all basic actions (Tax, Gather, Bribe, Arrest, Sanction, Coup) and
// every special action available to each role until a single winner remains.
//
// Note: You may need to adjust the PlayerFactory or direct constructor calls
// to match your actual codebase if method names differ.

#include "game_logic/Game.hpp"
#include "player/roles/Baron.hpp"
#include "player/roles/General.hpp"
#include "player/roles/Governor.hpp"
#include "player/roles/Judge.hpp"
#include "player/roles/Merchant.hpp"
#include "player/roles/Spy.hpp"
#include "player/PlayerFactory.hpp"
#include "player/RoleType.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace coup;
using namespace std;


int main() {
    // 1. Instantiate the Game
    Game game_1;

    // 2. Create one player of each role, in a fixed order.
    //    We assume PlayerFactory has a method createPlayer(name, game, roleType).
    //    Adjust if your signature differs.
    Baron* one    = new Baron("baron",    &game_1, RoleType::Baron);
    General* two  =new General("General",  &game_1, RoleType::General);
    Governor* tree = new Governor("Governor", &game_1, RoleType::Governor);
    Judge* four    = new Judge("Judge",    &game_1, RoleType::Judge);
    Merchant* five = new Merchant("Merchant", &game_1, RoleType::Merchant);
    Spy* six      = new Spy("Spy",      &game_1, RoleType::Spy);

    
    
        game_1.start();
    
    //ROUND:0
  
    one->tax();
    try {
        one->invest();
    } catch (const exception&) {
        // need at most 3 coins
    }
    two->gather();
    tree->gather();
    four->gather();
    five->gather();
    six->gather();

    
    six->peekCoinsCount(*one);
    six->blockArrest(*one);

     //ROUND:1
    one->tax();
    two->arrest(*one);
    try {
        tree->arrest(*one);
    } catch (const exception&) {
        // cannot arrest same player in same round
    }
    tree->tax();//get 3
    four->gather();
    five->receiveCoins(4);//just for demo! NOT AN REAL ACTION
    five->bribe(); 
    five->tax(); 
    try {
        four->deleteBribe(*five);
    } catch (const exception&) {
        // five already use his extra turn
    }
    five->tax(); 

    six->gather();

    //ROUND:2    
    one->invest();


    two->tax();
    tree->deleteTax(*two);
    tree->tax();
    try {
        two->arrest(*four);
    } catch (const exception&) {
        // not your turn
    }
    four->gather();
    five->gather();
    six->gather();

   //ROUND:3
    one->receiveCoins(7);//just for demo! NOT AN REAL ACTION
    one->coup(*tree);

    two->receiveCoins(4);//just for demo! NOT AN REAL ACTION
    two->deleteCoup(*tree);
    two->receiveCoins(4);//just for demo! NOT AN REAL ACTION

    two->bribe();
    
    four->deleteBribe(*two);

    two->receiveCoins(4);//just for demo! NOT AN REAL ACTION
    two->sanction(*tree);
    try {
        tree->tax();
    } catch (const exception&) {
        // cant perform tax
    }
    tree->receiveCoins(3);//just for demo! NOT AN REAL ACTION
    tree->arrest(*two);
    four->gather();
    five->gather();
    six->gather();

    // Print out the list of active players at this point.
    vector<string> remaining = game_1.players();
    for (const string& name : remaining) {
        cout << name << endl;
    }

    
    return 0;
}
