#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"

namespace coup {

/**
 * @brief Specialized player role: Baron.
 * 
 * The Baron can perform a special action "Invest":
 * pay 3 coins to receive 6.
 */
class Baron : public Player {
    public:
        /**
         * @brief Constructs a Baron with a name, game context, and role type.
         */
        Baron(const std::string& name, Game* game, RoleType role);
    
        /**
         * @brief Special ability: invest 3 coins to gain 6.
         * 
         * The action fails if the player does not have at least 3 coins.
         */
        void invest();
    
        /**
         * @brief Returns the list of special actions available to the Baron.
         */
        std::vector<SpecialActionInfo> getSpecialActions() override;
    
        /**
         * @brief Executes a specific special action of the Baron.
         * 
         * @param action The action to execute.
         * @param target An optional target (unused in Invest).
         */
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;
    };
    

}