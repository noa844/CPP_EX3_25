#include "Game.hpp"
#include "Status.hpp"
#include <stdexcept>
using namespace std;

namespace coup {

    void Game::start() {
        if (players.size() < 2) {
            throw std::runtime_error("The game requires at least 2 players to start.");
        }
        started = true;
    }
    
    bool Game::isStarted() const {
        return started;
    }
    
    bool Game::isOver() const {
        int activeCount = 0;
        for (Player* p : players) {
            if (p->isActive()) ++activeCount;
        }
        return activeCount <= 1;
    }

    void Game::addPlayer(Player* player) {
        if (players.size() >= 6) {
            throw std::runtime_error("Maximum number of players reached.");
        }
        // Ensure unique name
        for (Player* p : players) {
            if (p->getName() == player->getName()) {
                throw invalid_argument("Name already in use");
            }
        }

        players.push_back(player);
    }

    Player& Game::currentPlayer() const {
        return *players[currentTurnIndex];
    }

    void Game::nextTurn() {
        currentPlayer().resetStatuses();
        do {
            currentTurnIndex = (currentTurnIndex + 1) % players.size();
        } while (!players[currentTurnIndex]->isActive());

    }

    void Game::endTurn() {

        if (!currentPlayer().isStatusActive(Status::HasExtraAction)) {
            nextTurn(); 
        } else {
            currentPlayer().setStatus(Status::HasExtraAction, false); 
        }
    }

    vector<string> Game::activePlayers() const {
        vector<string> result;
        for (Player* p : players) {
            if(p->isActive()){
            result.push_back(p->getName());
            }
        }
        return result;
    }

    string Game::winner() const {
        if (players.size() != 1) {
            throw runtime_error("Game is still in progress");
        }
        return players.front()->getName();
    }

    void Game::eliminate(Player& target) {
        target.eliminate();
    }

}
