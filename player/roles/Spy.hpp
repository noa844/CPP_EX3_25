#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"


namespace coup {

    class Spy : public Player {

    public:
        Spy(const std::string& name, Game* game, RoleType role);

        int peekCoinsCount(const Player& target) const;
        void blockArrest(Player& target);   
        std::vector<SpecialActionInfo> getSpecialActions() override;
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;
    };

}
