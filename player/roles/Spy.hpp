#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"


namespace coup {
/**
 * @brief Specialized player role: Spy.
 * 
 * The Spy can peek at other players’ coins and block arrests.
 */
class Spy : public Player {
    public:
        /**
         * @brief Constructs a Spy with a name, game context, and role type.
         */
        Spy(const std::string& name, Game* game, RoleType role);
    
        /**
         * @brief Reveals the number of coins a player has.
         * 
         * @param target The player to inspect.
         * @return The number of coins they currently have.
         */
        int peekCoinsCount(const Player& target) const;
    
        /**
         * @brief Prevents another player from being arrested.
         * 
         * @param target The player being protected.
         */
        void blockArrest(Player& target);
    
        /**
         * @brief Returns the list of special actions available to the Spy.
         */
        std::vector<SpecialActionInfo> getSpecialActions() override;
    
        /**
         * @brief Executes a specific special action of the Spy.
         * 
         * @param action The action to execute.
         * @param target The relevant player (if required).
         */
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;
    };
    

}
