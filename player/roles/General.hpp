#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"



namespace coup {

    class General : public Player {
    public:
        General(const std::string& name, Game* game, RoleType role);

        void deleteCoup(Player& victim);

        std::vector<SpecialActionInfo> getSpecialActions() override;
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;


    };

}
