#pragma once
#include "../player/Player.hpp"

namespace coup {

    class Merchant : public Player {
    public:
        Merchant(const std::string& name, Game* game);

        void startTurnBonus();  
        void handleArrest();    
    };

}