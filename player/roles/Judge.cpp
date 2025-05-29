#include "Judge.hpp"
#include "../game_logic/Game.hpp"
#include <stdexcept>

namespace coup {

    Judge::Judge(const std::string& name, Game* game)
        : Player(name, game) {}

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

}
