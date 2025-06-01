#include "Merchant.hpp"
#include "../game_logic/Game.hpp"
#include "../player/RoleType.hpp"


#include <stdexcept>
#include <iostream>
using namespace std;

namespace coup {

    Merchant::Merchant(const std::string& name, Game* game ,RoleType role)
    : Player(name, game, role) {}

    void Merchant::startTurnBonus() {
        if (getCoinsCount() >= 3) {
            receiveCoins(1);
        }
    }

    void Merchant::handleArrest() {

        if(!this->hasEnoughCoins(2)){
            loseCoins(1);
            
        }else{
        loseCoins(2);
    
        }
        
    }


    

}