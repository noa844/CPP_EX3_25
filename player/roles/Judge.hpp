#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"


namespace coup {

    /**
 * @brief Specialized player role: Judge.
 * 
 * The Judge can cancel a Bribe action recently performed.
 */
class Judge : public Player {
    public:
        /**
         * @brief Constructs a Judge with a name, game context, and role type.
         */
        Judge(const std::string& name, Game* game, RoleType role);
    
        /**
         * @brief Cancels a recent Bribe action performed by the target player.
         * 
         * @param target The player whose bribe will be cancelled.
         */
        void deleteBribe(Player& target);
    
        /**
         * @brief Returns the list of special actions available to the Judge.
         */
        std::vector<SpecialActionInfo> getSpecialActions() override;
    
        /**
         * @brief Executes a specific special action of the Judge.
         * 
         * @param action The action to execute.
         * @param target The player whose bribe may be cancelled.
         */
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;
    };
    
}
