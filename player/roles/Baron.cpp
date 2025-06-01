#include "Baron.hpp"
#include "../player/RoleType.hpp"
#include "../game_logic/Game.hpp"
#include <stdexcept>

using namespace std;


namespace coup {

    Baron::Baron(const std::string& name, Game* game, RoleType role)
        : Player(name, game, role) {}

    void Baron::invest() {
        checkGameIsActive();

        if (!hasEnoughCoins(3)) {
            throw std::runtime_error("Not enough coins to invest.");
        }

        loseCoins(3);
        receiveCoins(6);

        game->endTurn();
    }

    std::vector<SpecialActionInfo> Baron::getSpecialActions() {
        return {
            {AllSpecialActionType::Invest, false, false}
    
        };
    }

    void Baron::executeSpecialAction(AllSpecialActionType action, Player* target) {
        switch (action) {
            case AllSpecialActionType::Invest:
               invest();
               break;
            default:
                throw runtime_error("Action not handled.");
        }
    }
}