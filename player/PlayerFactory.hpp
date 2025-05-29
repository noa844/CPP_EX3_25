#pragma once

#include "../game_logic/Game.hpp"
#include "../player/roles/Governor.hpp"
#include "../player/roles/Spy.hpp"
#include "../player/roles/Baron.hpp"
#include "../player/roles/Merchant.hpp"
#include "../player/roles/General.hpp"
#include "../player/roles/Judge.hpp"
#include <string>
#include <cstdlib>  
  

namespace coup {

    class PlayerFactory {
    public:
        static Player* createPlayer(const std::string& name, Game* game);
    };

}
