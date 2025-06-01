#include "General.hpp"
#include "../game_logic/Game.hpp"
#include "../player/RoleType.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    General::General(const std::string& name, Game* game ,RoleType role)
    : Player(name, game, role) {}

    void General::deleteCoup(Player& victim) {
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

    std::vector<SpecialActionInfo> General::getSpecialActions() {
        return {
            {AllSpecialActionType::Delete_coup, true, true}
    
        };
    }

    void General::executeSpecialAction(AllSpecialActionType action, Player* target) {
        if (!target) throw runtime_error("Target required.");

        switch (action) {
            case AllSpecialActionType::Delete_coup:
               deleteCoup(*target);
               break;
            default:
                throw runtime_error("Action not handled.");
        }
    }
}

