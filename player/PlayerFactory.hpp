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

    /**
 * @brief Factory class to create players with random roles.
 * 
 * Used during game setup to assign a role to each player dynamically.
 */
class PlayerFactory {
    public:
        /**
         * @brief Creates a new player with a randomly chosen role.
         * 
         * @param name The name of the player.
         * @param game A pointer to the current game instance.
         * @return Pointer to a dynamically allocated Player object.
         */
        static Player* createPlayer(const std::string& name, Game* game);
    };
    

}
