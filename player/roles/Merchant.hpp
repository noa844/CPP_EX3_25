#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"



namespace coup {

   /**
 * @brief Specialized player role: Merchant.
 * 
 * The Merchant earns a bonus at the beginning of each turn,
 * and has specific behavior when arrested.
 */
class Merchant : public Player {
    public:
        /**
         * @brief Constructs a Merchant with a name, game context, and role type.
         */
        Merchant(const std::string& name, Game* game, RoleType role);
    
        /**
         * @brief Called at the start of the Merchant's turn.
         * 
         * Adds a coin or bonus before action selection.
         */
        void startTurnBonus();
    
        /**
         * @brief Called when the Merchant is arrested.
         * 
         * Can trigger custom logic or immunity.
         */
        void handleArrest();
    };
    
}