#pragma once
#include <string>
#include <unordered_map>
#include "Status.hpp"
#include "RoleType.hpp"
#include "SpecialActionUtils.hpp"
#include "../lib/magic_enum.hpp"



namespace coup {
    class Game;
}


namespace coup {

 /**
 * @brief Represents a player in the Coup game.
 * 
 * Each player has a name, a number of coins, a role (possibly specialized via subclass),
 * and a set of statuses affecting gameplay. This class defines core player behavior
 * (gathering, bribing, taxing, etc.) and supports overriding for special roles.
 */
class Player {
    protected:
        std::string name; ///< Name of the player.
        Game* game; ///< Pointer to the game instance this player belongs to.
        int coinsCount = 0; ///< Current number of coins the player holds.
        bool active = true; ///< Indicates whether the player is still active (not eliminated).
        std::unordered_map<Status, bool> statusMap; ///< Map of status effects applied to this player.
        RoleType roleType = RoleType::None; ///< Enum representing the player's role.
    
    public:
        /**
         * @brief Constructs a player with a name, game reference, and role type.
         * 
         * @param name Name of the player.
         * @param game Pointer to the game object.
         * @param roleType The role type assigned to this player.
         */
        Player(const std::string& name, Game* game, RoleType roleType);
    
        /**
         * @brief Virtual destructor for cleanup in derived classes.
         */
        virtual ~Player();
    
        // === Coin operations ===
    
        /**
         * @brief Add coins to the player's total.
         * 
         * @param amount Number of coins to add.
         */
        void receiveCoins(int amount);
    
        /**
         * @brief Remove coins from the player's total.
         * 
         * @param amount Number of coins to subtract.
         */
        void loseCoins(int amount);
    
        /**
         * @brief Returns the current number of coins.
         * 
         * @return Current coin count.
         */
        int getCoinsCount() const;
    
        /**
         * @brief Checks whether the player has enough coins.
         * 
         * @param amount The required number of coins.
         * @return true if the player has at least 'amount' coins.
         */
        bool hasEnoughCoins(int amount) const;
    
        // === Info getters ===
    
        std::string getName() const;          ///< Returns the player's name.
        RoleType getRoleType() const;         ///< Returns the role type enum.
        std::string getRoleName() const;      ///< Returns the string representation of the role.
    
        // === Status & activity ===
    
        bool isActive() const;                ///< Returns true if the player is still in the game.
        void eliminate();                     ///< Marks player as eliminated.
        void revive();                        ///< Revives an eliminated player.
    
        /**
         * @brief Initializes all known statuses to false.
         */
        void initializeStatusMap();
    
        /**
         * @brief Sets a status to active or inactive.
         * 
         * @param s The status enum.
         * @param value true to activate, false to deactivate.
         */
        void setStatus(Status s, bool value);
    
        /**
         * @brief Checks if a given status is active.
         * 
         * @param s The status enum.
         * @return true if the status is currently active.
         */
        bool isStatusActive(Status s) const;
    
        /**
         * @brief Resets all statuses to false.
         */
        void resetStatuses();
    
        /**
         * @brief Sets the coin count to 0.
         */
        void resetCoinsCount();
    
        // === Game checks ===
    
        /**
         * @brief Throws an error if the game is not started or already ended.
         */
        void checkGameIsActive() const;
    
        /**
         * @brief Throws an error if it's not this player's turn.
         */
        void checkPlayerTurn() const;
    
        /**
         * @brief Checks if the player must perform a coup (e.g. has too many coins).
         * 
         * @return true if the player is forced to play Coup.
         */
        bool mustPerformCoup() const;
    
        /**
         * @brief Checks if the player's last action was a bribe.
         * 
         * @return true if last action was bribe.
         */
        bool lastActionIsBribe() const;
    
        /**
         * @brief Checks if the player has at least the specified number of coins.
         * 
         * @param coins The number of coins needed for a coup.
         * @return true if the player has enough.
         */
        bool hasEnoughCoinForCoup(int coins) const;
    
        // === Special actions ===
    
        /**
         * @brief Returns the list of special actions available for this player.
         * 
         * Can be overridden by subclasses to define role-specific abilities.
         * 
         * @return A list of SpecialActionInfo objects.
         */
        virtual std::vector<SpecialActionInfo> getSpecialActions();
    
        /**
         * @brief Executes a special action (optionally on a target).
         * 
         * Default behavior does nothing; to be overridden by subclasses.
         * 
         * @param action The type of special action.
         * @param target Optional player target.
         */
        virtual void executeSpecialAction(AllSpecialActionType action, Player* target = nullptr);
    
        // === Game actions ===
    
        /**
         * @brief Take 1 coin from the bank.
         */
        void gather();
    
        /**
         * @brief Take 2 coins (or 3 if Governor).
         */
        virtual void tax();
    
        /**
         * @brief Pay 4 coins to unlock an additional action.
         */
        void bribe();
    
        /**
         * @brief Steal 1 coin from another player.
         * 
         * Can behave differently depending on the target's role.
         * 
         * @param target The player to arrest.
         */
        virtual void arrest(Player& target);
    
        /**
         * @brief Block another player from performing gather/tax.
         * 
         * @param target The player to sanction.
         */
        virtual void sanction(Player& target);
    
        /**
         * @brief Eliminate another player from the game.
         * 
         * @param target The player to eliminate.
         */
        void coup(Player& target);
    };
}    