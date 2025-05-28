#pragma once
#include <vector>
#include <deque>
#include <string>
#include "ActionType.hpp"
#include "DeletableActionType.hpp"
#include "../player/Player.hpp"




namespace coup {
    
    struct Action {
        std::string playerName;
        ActionType action;
        DeletableActionType type;
        int turn;
    };
    

    class Game {
    private:

        std::vector<Player*> players;
        size_t currentTurnIndex = 0;
        int turnCounter;
        bool started = false;
        std::string lastArrested;
        std::deque<Action> actionHistory;
        std::vector<std::string> activePlayersList;
        
        

      
    public:

        Game() {};

        void start();                 
        bool isStarted() const;      
        bool isOver() const; 

        const std::string& getLastArrested() const;
        void setLastArrested(const std::string& name);

        void addPlayer(Player* player);            // Add a player with unique name
        Player& currentPlayer() const;             // Who plays now
        bool isPlayerTurn(const Player* player) const;

        void nextTurn();                           // Advance turn
        void endTurn();                            //advance trun if player finish all his actions 

        const int getActionHistorySize() const;
        void logAction(const std::string& playerName, ActionType action, DeletableActionType type);
        bool hasRecentDeletableAction(const std::string& playerName, DeletableActionType type) const;
        void removeRecentDeletableAction(const std::string& playerName, DeletableActionType type);
        void erasePlayerAction(const std::string& playerName);

        std::vector<std::string> activePlayers() const;
        std::string winner() const;


        // Optional: remove player after coup
        void eliminate(Player& target);
    };

}
