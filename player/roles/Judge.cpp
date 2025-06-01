#include "Judge.hpp"
#include "../game_logic/Game.hpp"
#include "../player/RoleType.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Judge::Judge(const std::string& name, Game* game ,RoleType role)
    : Player(name, game, role) {}

    void Judge::deleteBribe(Player& target) {
        checkGameIsActive();

        if (!game->hasRecentDeletableAction(target.getName(), DeletableActionType::Bribe)) {
            throw std::runtime_error("No recent bribe to block for this player.");
        }
        size_t bribeRound = game->getRoundCounter();
        for (const auto& action : game->getActionHistory()) {
            if (action.playerName == target.getName() &&
                action.turn == bribeRound &&
                action.action != ActionType::Bribe) {
                throw std::runtime_error("Cannot cancel bribe: player already used bonus action.");
            }
        }

        target.setStatus(Status::HasExtraAction, false);
        game->removeRecentDeletableAction(target.getName(), DeletableActionType::Bribe);
    }

    
    std::vector<SpecialActionInfo> Judge::getSpecialActions() {
        return {
            {AllSpecialActionType::Delete_bribe, true, true}
    
        };
    }

    void Judge::executeSpecialAction(AllSpecialActionType action, Player* target) {
        if (!target) throw runtime_error("Target required.");

        switch (action) {
            case AllSpecialActionType::Delete_bribe:
               deleteBribe(*target);
                break;
            default:
                throw runtime_error("Action not handled.");
        }
    }

}
