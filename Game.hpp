#pragma once
#include <vector>
#include <string>
#include "Player.hpp"



namespace coup {

    class Game {
    private:
        std::vector<Player*> players;
        size_t currentTurnIndex = 0;
        bool started = false;
    public:

        Game() {};

        void start();                 
        bool isStarted() const;      
        bool isOver() const; 

        void addPlayer(Player* player);            // Add a player with unique name
        Player& currentPlayer() const;             // Who plays now
        void nextTurn();                           // Advance turn
        void endTurn();                            //advance trun if player finish all his actions 
        std::vector<std::string> activePlayers() const;
        std::string winner() const;

        // Optional: remove player after coup
        void eliminate(Player& target);
    };

}
