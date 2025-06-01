#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"


namespace coup {

    class Governor : public Player {
    public:
        Governor(const std::string& name, Game* game, RoleType role);

        // Override tax behavior
        void tax() override;
        void deleteTax(Player& target);
        std::vector<SpecialActionInfo> getSpecialActions() override;
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;


    };

}
