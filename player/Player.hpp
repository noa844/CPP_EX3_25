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

    class Player {
    protected:
        std::string name;
        Game* game;
        int coinsCount = 0;
        bool active = true; // Indicates whether the player is still in the game
        std::unordered_map<Status, bool> statusMap;
        RoleType roleType = RoleType::None;

    
    public:
        //constructor
        Player(const std::string& name, Game* game, RoleType roleType);

        //destructor
        virtual ~Player();
        
        // Coin-related methods
        void receiveCoins(int amount);     // Add coins to the player
        void loseCoins(int amount);        // Remove coins from the player
        int getCoinsCount() const;         // Return current coin count
        bool hasEnoughCoins(int amount) const;

        std::string getName() const;
        RoleType getRoleType() const;
        std::string getRoleName() const; 

        bool isActive() const;             // Check if the player is still active
        void eliminate();                  // Mark the player as eliminated
        void revive();
        
        void initializeStatusMap();
        void setStatus(Status s, bool value);
        bool isStatusActive(Status s) const;
        void resetStatuses(); 
        void resetCoinsCount();

        void checkGameIsActive() const;
        void checkPlayerTurn()const;

        bool mustPerformCoup() const;
        bool lastActionIsBribe() const;
        bool hasEnoughCoinForCoup(int coins) const;

        virtual std::vector<SpecialActionInfo> getSpecialActions();

        virtual void executeSpecialAction(AllSpecialActionType action, Player* target = nullptr);


        // Game actions (can be overridden in subclasses)
        void gather();             // Take 1 coin from the bank
        virtual void tax();        // Take 2 coins (3 for Governor)
        void bribe();              // Pay 4 coins to perform another action
        virtual void arrest(Player& target);   // Steal 1 coin from another player
        virtual void sanction(Player& target); // Block another player from gather/tax
        void coup(Player& target);     // Eliminate another player from the game
 

        
    };

}
    