#include "Game.hpp"
#include "../player/Status.hpp"
#include "../player/roles/Merchant.hpp"
#include "../lib/magic_enum.hpp"
#include "../player/PlayerFactory.hpp"
#include "ActionType.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;
using std::cout;
using std::endl;

namespace coup {

    Game::~Game() {
        for (Player* p : playersList) {
            delete p;
        }
    }

    void Game::start() {
        if (playersList.size() < 2) {
            throw runtime_error("The game requires at least 2 players to start.");
        }
        started = true;
        std::cout << "Game: start" << std::endl;
    }
    
    bool Game::isStarted() const {
        return started;
    }
    
    bool Game::isOver() const {
        int activeCount = 0;
        for (Player* p : playersList) {
            if (p->isActive()) ++activeCount;
        }
        if(activeCount <=1 && isStarted()){
            return true;
        }else { 
            return false;
        }
        
    }

    void Game::restart() {
        
  
        cout << "[LOG] Game::restart -> restarting the game from scratch." << endl;
        
        started = false;
        
        for (Player* p : playersList) {
            delete p;
        }
        playersList.clear();
        currentTurnIndex = 0;
        roundCounter = 0;
        lastArrested = "";    
        actionHistory.clear();      
        playersAddedCount = static_cast<int>(playersList.size());
        activePlayersList.clear();

       
        cout << "[LOG] All players have been reset to 0 coins & active. "
             << "Ready to start a new game." << endl;
    }


    int Game::getStartPlayerCount() const {
        return playersAddedCount;
    }

    void Game::createPlayerRandomRole(const std::string& name) {
        if (playersAddedCount >= 6) {
            throw std::runtime_error("Maximum number of players reached.");
        }
    
        Player* newPlayer = PlayerFactory::createPlayer(name, this); 
        std::cout << "Game: create Player( name=\"" << name << "\" )" << std::endl;
    }
    
    Player& Game::getPlayerByName(const std::string& name) {
        for (auto* p : playersList) {
            if (p->getName() == name) return *p;
        }
        throw std::runtime_error("No such player: " + name);
    }
    
    const std::vector<Player*>& Game::getPlayers() const {
        return playersList;
    }
    

    const string& Game::getLastArrested() const {
        return lastArrested;
    }
    
    void Game::setLastArrested(const string& name) {
        lastArrested = name;
    }    

    void Game::addPlayer(Player* player) {
        if (playersAddedCount >= 6) {
            throw runtime_error("Maximum number of players reached.");
        }
        // Ensure unique name
        for (Player* p : playersList) {
            if (p->getName() == player->getName()) {
                throw invalid_argument("Name already in use");
            }
        }

        playersList.push_back(player);
        ++playersAddedCount;
       
    }

    Player& Game::currentPlayer() const {
        return *playersList[currentTurnIndex];
    }

    size_t Game::getCurrentPlayerIndex() const{
        return currentTurnIndex;
    }

    bool Game::isPlayerTurn(const Player* player)const{
        return playersList[currentTurnIndex] == player;
    }

    void Game::nextTurn() {
        currentPlayer().resetStatuses();

        do {
            currentTurnIndex = (currentTurnIndex + 1) % playersList.size();
        } while (!playersList[currentTurnIndex]->isActive());

        if (currentTurnIndex == 0) {
            ++roundCounter;
        }
        Player* next = playersList[currentTurnIndex];
        if (Merchant* m = dynamic_cast<Merchant*>(next)) {
            m->startTurnBonus();
        }
        std::cout << "Game: next Turn: " << next->getName() <<std::endl; 
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

    const std::deque<Action>& Game::getActionHistory() const {
        return actionHistory;
    }
    
    void Game::logAction(const string& playerName, ActionType action, DeletableActionType type,const std::string& targetName) {
        
        for (auto it = actionHistory.begin(); it != actionHistory.end(); ) {
            if (it->playerName == playerName && it->turn < roundCounter) {
                it = actionHistory.erase(it);  
            } else {
                ++it;
            }
        }
        actionHistory.push_back({playerName, action, type,roundCounter,targetName});
        std::cout << "[Action]: "
        << playerName
        << " performed " << toString(action)
        << " at round " << roundCounter;

        if (targetName != "") {
        std::cout << " on " << targetName;
        }

        std::cout << std::endl;
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

    bool Game::wasCoupedRecently(const std::string& name) const {
        size_t t = roundCounter;
        
        for (const auto& a : actionHistory) {
            
            if (a.action == ActionType::Coup && a.target == name &&
                (a.turn == t || a.turn == t - 1)) {
                return true;
            }
        }
        return false;
    }  
    
    bool Game::requiresTarget(ActionType action) {
        return action == ActionType::Coup || 
               action == ActionType::Arrest || 
               action == ActionType::Sanction;
    }
    
    size_t Game::getRoundCounter() const {
        return roundCounter;
    }

    vector<string> Game::activePlayers() const {
        vector<string> result;
        for (Player* p : playersList) {
            if(p->isActive()){
            result.push_back(p->getName());
            }
        }
        return result;
    }

    string Game::winner() const {
        if (!isOver()) {
            throw runtime_error("Game is still in progress");
        }else{
        vector<string> winner = activePlayers();
        return winner.front();
        }
    }

    void Game::eliminate(Player& target) {
        target.eliminate();
        target.resetCoinsCount();
        erasePlayerAction(target.getName());
        target.resetStatuses();
        std::cout << "Game: eliminate( target=" << target.getName() << " )" << std::endl;
    }
    void Game::revive(Player& victim){
        victim.revive();
        std::cout << "Game: revive( victim=" << victim.getName() << " )" << std::endl;

    }
    std::string Game::turn() const {
        vector<string> activeNames;
        activeNames = activePlayers();
        if (activeNames.empty() || !started) {
            throw std::runtime_error("Game has not started or there are no players.");
        }
        return currentPlayer().getName();
    }
    
    vector<string> Game::players() const {
        vector<string> activeNames;
        activeNames = activePlayers();
        return activeNames;
    }
    

}
