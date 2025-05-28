#include "Governor.hpp"
#include "../game_logic/Game.hpp"
#include <stdexcept>

namespace coup {

    Governor::Governor(const std::string& name, Game* game)
        : Player(name, game) {}

    void Governor::tax() {
        checkGameIsActive();
        checkPlayerTurn();

        if (isStatusActive(Status::BlockedForTax)) {
            throw std::runtime_error("You are blocked to perform this action.");
        }

        receiveCoins(3);  // Special: Governor gets 3 coins
        game->endTurn();
    }

    void Governor::deleteTax(Player& target) {
        checkGameIsActive();
    
        if (!game->hasRecentDeletableAction(target.getName(), DeletableActionType::Tax)) {
            throw std::runtime_error("No recent tax to block for this player.");
        }
    
        target.loseCoins(2);
        game->removeRecentDeletableAction(target.getName(), DeletableActionType::Tax);
    
        game->endTurn();
    }
    

}
