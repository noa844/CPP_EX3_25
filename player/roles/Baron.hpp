#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"

namespace coup {

    class Baron : public Player {
    public:
        Baron(const std::string& name, Game* game, RoleType role);

        void invest(); // Pay 3 to get 6

        std::vector<SpecialActionInfo> getSpecialActions() override;
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;

        
    };

}