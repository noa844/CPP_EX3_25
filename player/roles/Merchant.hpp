#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"



namespace coup {

    class Merchant : public Player {
    public:
        Merchant(const std::string& name, Game* game, RoleType role);

        void startTurnBonus();  
        void handleArrest();   
        

    };

}