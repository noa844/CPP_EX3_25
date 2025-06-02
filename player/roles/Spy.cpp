#include "Spy.hpp"
#include "../player/Player.hpp"

#include "../game_logic/Game.hpp"
#include "../player/RoleType.hpp"

#include "../player/SpecialActionUtils.hpp"
#include <iostream>
#include <stdexcept>


using namespace std;


namespace coup {

    Spy::Spy(const string& name, Game* game, RoleType role)
    : Player(name, game, role) {}

    int Spy::peekCoinsCount(const Player& target) const {
        checkGameIsActive();
        std::cout <<"coins: "<<target.getCoinsCount()<< std::endl;
        return target.getCoinsCount();
       
    }

    void Spy::blockArrest(Player& target) {
        checkGameIsActive();
        target.setStatus(Status::BlockedForArrest, true);
    }
    
    vector<SpecialActionInfo> Spy::getSpecialActions() {
        return {
            {AllSpecialActionType::Peek_Coins, true, false},
            {AllSpecialActionType::Block_Arrest, true, false}
        };
    }

    void Spy::executeSpecialAction(AllSpecialActionType action, Player* target) {
        if (!target) throw runtime_error("Target required.");

        switch (action) {
            case AllSpecialActionType::Peek_Coins:
                peekCoinsCount(*target);
                break;
            case AllSpecialActionType::Block_Arrest:
                blockArrest(*target);
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