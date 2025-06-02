#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"


namespace coup {
/**
 * @brief Specialized player role: Governor.
 * 
 * The Governor collects more coins on tax and can cancel another player's tax.
 */
class Governor : public Player {
    public:
        /**
         * @brief Constructs a Governor with a name, game context, and role type.
         */
        Governor(const std::string& name, Game* game, RoleType role);
    
        /**
         * @brief Overrides tax to give 3 coins instead of 2.
         */
        void tax() override;
    
        /**
         * @brief Cancels the last tax performed by another player.
         * 
         * @param target The player whose tax will be deleted.
         */
        void deleteTax(Player& target);
    
        /**
         * @brief Returns the list of special actions available to the Governor.
         */
        std::vector<SpecialActionInfo> getSpecialActions() override;
    
        /**
         * @brief Executes a specific special action of the Governor.
         * 
         * @param action The action to execute.
         * @param target The player whose tax might be deleted.
         */
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;
    };
    

}
