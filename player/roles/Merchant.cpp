#include "Merchant.hpp"
#include "../game_logic/Game.hpp"
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
        std::cout << "[Merchant] Coins before arrest: " << getCoinsCount() << "\n";

        if(!this->hasEnoughCoins(2)){
            loseCoins(1);
            std::cout << "[Merchant] Losing 1 coin\n";
        }else{
        loseCoins(2);
        std::cout << "[Merchant] Losing 2 coins\n";
        }
        
    }

}