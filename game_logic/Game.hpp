#pragma once
#include <vector>
#include <deque>
#include <string>
#include "ActionType.hpp"
#include "DeletableActionType.hpp"
#include "../player/Player.hpp"
#include "../player/PlayerFactory.hpp"




namespace coup {
    
    struct Action {
        std::string playerName;
        ActionType action;
        DeletableActionType type;
        size_t turn;
        std::string target = "";
    };
    

    class Game {
    private:

        std::vector<Player*> players;
        size_t currentTurnIndex = 0;
        size_t roundCounter = 0;
        bool started = false;
        std::string lastArrested;
        std::deque<Action> actionHistory;
        std::vector<std::string> activePlayersList;
        int playersAddedCount = 0;
        
        

      
    public:

        Game() {};
        ~Game();

        Game(const Game& other) = delete;
        Game& operator=(const Game& other) = delete;
        Game(Game&& other) = delete;
        Game& operator=(Game&& other) = delete;

        void start();                 
        bool isStarted() const;      
        bool isOver() const; 
        int getStartPlayerCount() const;
        void createPlayerRandomRole(const std::string& name);
        Player& getPlayerByName(const std::string& name);
        const std::vector<Player*>& getPlayers() const;

        const std::string& getLastArrested() const;
        void setLastArrested(const std::string& name);

        void addPlayer(Player* player);            // Add a player with unique name
        Player& currentPlayer() const;             // Who plays now
        bool isPlayerTurn(const Player* player) const;

        void nextTurn();                           // Advance turn
        void endTurn();                            //advance trun if player finish all his actions 

        const int getActionHistorySize() const;
        const std::deque<Action>& getActionHistory() const;
        void logAction(const std::string& playerName, ActionType action, DeletableActionType type,const std::string& targetName = "");
        bool hasRecentDeletableAction(const std::string& playerName, DeletableActionType type) const;
        void removeRecentDeletableAction(const std::string& playerName, DeletableActionType type);
        void erasePlayerAction(const std::string& playerName);

        bool wasCoupedRecently(const std::string& name) const;
        size_t getRoundCounter() const;

        std::vector<std::string> activePlayers() const;
        std::string winner() const;


        // Optional: remove player after coup
        void eliminate(Player& target);
        void revive(Player& victim);
    };

}
