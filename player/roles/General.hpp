#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"
#include "../player/SpecialActionUtils.hpp"



namespace coup {

    /**
 * @brief Specialized player role: General.
 * 
 * The General can block a Coup and revive a recently eliminated player.
 */
class General : public Player {
    public:
        /**
         * @brief Constructs a General with a name, game context, and role type.
         */
        General(const std::string& name, Game* game, RoleType role);
    
        /**
         * @brief Cancels a Coup action against a victim and revives them.
         * 
         * This can only be used if the victim was just Coupled.
         * 
         * @param victim The player to revive.
         */
        void deleteCoup(Player& victim);
    
        /**
         * @brief Returns the list of special actions available to the General.
         */
        std::vector<SpecialActionInfo> getSpecialActions() override;
    
        /**
         * @brief Executes a specific special action of the General.
         * 
         * @param action The action to execute.
         * @param target The player to revive (required).
         */
        void executeSpecialAction(AllSpecialActionType action, Player* target) override;
    };
    

}
