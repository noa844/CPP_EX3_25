#include "General.hpp"
#include "../game_logic/Game.hpp"
#include "../player/RoleType.hpp"

#include "../player/SpecialActionUtils.hpp"

#include <stdexcept>
#include <iostream>

using namespace std;

namespace coup {

    General::General(const std::string& name, Game* game ,RoleType role)
    : Player(name, game, role) {}

    void General::deleteCoup(Player& victim) {
        std::cout<<"delete coup"<<std::endl;
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
               std::cout << "Executing special action: " << magic_enum::enum_name(action) << std::endl;

               break;
            default:
                throw runtime_error("Action not handled.");
        }
        std::cout << "[Action]: "
        << this->getName()
        << " performed " << toString(action);

        if (target != nullptr) {
        std::cout << " on " << target->getName();
        }

        std::cout << std::endl;
    }
}

