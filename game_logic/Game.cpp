#include "Game.hpp"
#include "../player/Status.hpp"
#include "../lib/magic_enum.hpp"
#include <stdexcept>
using namespace std;

namespace coup {

    void Game::start() {
        if (players.size() < 2) {
            throw std::runtime_error("The game requires at least 2 players to start.");
        }
        started = true;
        setActionHistoryLimit(players.size());
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

    const std::string& Game::getLastArrested() const {
        return lastArrested;
    }
    
    void Game::setLastArrested(const std::string& name) {
        lastArrested = name;
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

    bool Game::isPlayerTurn(const Player* player)const{
        return players[currentTurnIndex] == player;
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

    void Game::setActionHistoryLimit(size_t max) {
        actionHistoryMaxSize = max;
        actionHistory.clear();
    }
    
    void Game::logAction(const std::string& playerName, ActionType action, DeletableActionType type) {
        if (actionHistory.size() >= actionHistoryMaxSize) {
            actionHistory.pop_front();
        }
        actionHistory.push_back({playerName, action, type});
    }
    
    bool Game::hasRecentDeletableAction(const std::string& playerName, DeletableActionType type) const {
        for (const auto& action : actionHistory) {
            if (action.playerName == playerName && action.type == type) {
                return true;
            }
        }
        return false;
    }
    
    void Game::removeRecentDeletableAction(const std::string& playerName, DeletableActionType type) {
        for (auto it = actionHistory.begin(); it != actionHistory.end(); ++it) {
            if (it->playerName == playerName && it->type == type) {
                actionHistory.erase(it);
                break;
            }
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
