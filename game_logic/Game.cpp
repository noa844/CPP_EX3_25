#include "Game.hpp"
#include "../player/Status.hpp"
#include "../lib/magic_enum.hpp"
#include <stdexcept>
using namespace std;

namespace coup {

    void Game::start() {
        if (players.size() < 2) {
            throw runtime_error("The game requires at least 2 players to start.");
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

    const string& Game::getLastArrested() const {
        return lastArrested;
    }
    
    void Game::setLastArrested(const string& name) {
        lastArrested = name;
    }    

    void Game::addPlayer(Player* player) {
        if (players.size() >= 6) {
            throw runtime_error("Maximum number of players reached.");
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

        if (currentTurnIndex == 0) {
            ++turnCounter;
        }
        

    }

    void Game::endTurn() {

        if (!currentPlayer().isStatusActive(Status::HasExtraAction)) {
            nextTurn(); 
        } else {
            currentPlayer().setStatus(Status::HasExtraAction, false); 
        }
    }

   const int Game::getActionHistorySize() const{
    return actionHistory.size();
   }
    
    void Game::logAction(const string& playerName, ActionType action, DeletableActionType type) {
        
        for (auto it = actionHistory.begin(); it != actionHistory.end(); ) {
            if (it->playerName == playerName && it->turn < turnCounter) {
                it = actionHistory.erase(it);  
            } else {
                ++it;
            }
        }
        actionHistory.push_back({playerName, action, type,turnCounter});
    }
    
    bool Game::hasRecentDeletableAction(const string& playerName, DeletableActionType type) const {
        for (const auto& action : actionHistory) {
            if (action.playerName == playerName && action.type == type) {
                return true;
            }
        }
        return false;
    }
    
    void Game::removeRecentDeletableAction(const string& playerName, DeletableActionType type) {
        for (auto it = actionHistory.begin(); it != actionHistory.end(); ++it) {
            if (it->playerName == playerName && it->type == type) {
                it->type = DeletableActionType::None;
                it->action = ActionType::None;
            }
        }
    }

    void Game::erasePlayerAction(const string& playerName){
        for (auto it = actionHistory.begin(); it != actionHistory.end(); ) {
            if (it->playerName == playerName) {
                it = actionHistory.erase(it);  
            } else {
                ++it;
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
        erasePlayerAction(target.getName());
        
    }

}
