#include "Governor.hpp"
#include "../game_logic/Game.hpp"
#include "../player/RoleType.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Governor::Governor(const std::string& name, Game* game ,RoleType role)
    : Player(name, game, role) {}

    void Governor::tax() {
        checkGameIsActive();
        checkPlayerTurn();

        if(mustPerformCoup()){
            throw runtime_error("You have to perform coup instead of this action.");
        }

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

    std::vector<SpecialActionInfo> Governor::getSpecialActions() {
        return {
            {AllSpecialActionType::Delete_tax, true, true}
    
        };
    }

    void Governor::executeSpecialAction(AllSpecialActionType action, Player* target) {
        if (!target) throw runtime_error("Target required.");

        switch (action) {
            case AllSpecialActionType::Delete_tax:
               deleteTax(*target);
               break;
            default:
                throw runtime_error("Action not handled.");
        }
    }
}
    


