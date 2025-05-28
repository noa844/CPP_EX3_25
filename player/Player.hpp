#pragma once
#include <string>
#include <unordered_map>
#include "Status.hpp"

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

    
    public:
        //constructor
        Player(const std::string& name, Game* game);

        //destructor
        virtual ~Player() = default;
        
        // Coin-related methods
        void receiveCoins(int amount);     // Add coins to the player
        void loseCoins(int amount);        // Remove coins from the player
        int getCoinsCount() const;         // Return current coin count
        bool hasEnoughCoins(int amount) const;

        std::string getName() const;

        bool isActive() const;             // Check if the player is still active
        void eliminate();                  // Mark the player as eliminated
        
        void initializeStatusMap();
        void setStatus(Status s, bool value);
        bool isStatusActive(Status s);
        void resetStatuses(); 

        void checkGameIsActive() const;
        void checkPlayerTurn()const;

        // Game actions (can be overridden in subclasses)
        void gather();             // Take 1 coin from the bank
        virtual void tax();        // Take 2 coins (3 for Governor)
        void bribe();              // Pay 4 coins to perform another action
        virtual void arrest(Player& target);   // Steal 1 coin from another player
        virtual void sanction(Player& target); // Block another player from gather/tax
        void coup(Player& target);     // Eliminate another player from the game
 

        
    };

}
    