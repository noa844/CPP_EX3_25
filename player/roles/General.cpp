#include "General.hpp"
#include "../game_logic/Game.hpp"
#include <stdexcept>

namespace coup {

    General::General(const std::string& name, Game* game)
        : Player(name, game) {}

    void General::blockCoup(Player& victim) {
        checkGameIsActive();

        if (!hasEnoughCoins(5)) {
            throw std::runtime_error("Not enough coins to block the coup.");
        }
        

        if (!game->wasCoupedRecently(victim.getName())) {
            throw std::runtime_error("No recent coup on this player to block.");
        }

        loseCoins(5); 
        game->revive(victim);

          
    }

}
