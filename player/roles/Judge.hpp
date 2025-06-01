#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"


namespace coup {

    class Judge : public Player {
    public:
        Judge(const std::string& name, Game* game, RoleType role);

        void deleteBribe(Player& target); 
        std::vector<SpecialActionInfo> getSpecialActions() override;
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;


    };

}
