#include "Player.hpp"
#include "../game_logic/Game.hpp"
#include "Status.hpp"
#include "../lib/magic_enum.hpp"

#include <stdexcept>
using namespace std;
using coup::Status;
using namespace magic_enum;



namespace coup {

    Player::Player(const string& name, Game* game) : name(name),game(game), coinsCount(0),active(true) {
        initializeStatusMap();
        game->addPlayer(this);
    }

    Player::~Player() {}


    void Player::receiveCoins(int amount) {
        if (amount < 0) throw invalid_argument("Cannot receive negative amount of coins.");
        coinsCount += amount;
    }

    void Player::loseCoins(int amount) {
        if (amount > coinsCount) throw runtime_error("Not enough coins.");
        coinsCount -= amount;
    }

    int Player::getCoinsCount() const {
        return coinsCount;
    }

    bool Player::hasEnoughCoins(int amount) const{
        if(coinsCount >= amount){
            return true;
        }else{
            return false;
        }
    }

    string Player::getName() const {
        return name;
    }

    bool Player::isActive() const {
        return active;
    }

    void Player::eliminate() {
        active = false;
    }

    void Player::initializeStatusMap(){

        for (Status s : magic_enum::enum_values<Status>()) {
            statusMap[s] = false;
        }
        
    }

    void Player::setStatus(Status s, bool value) {
        statusMap.at(s) = value;
    } 

    void Player::resetStatuses() {
        for (auto& element : statusMap) {
            element.second = false;
        }
    }    

    bool Player::isStatusActive(Status s){
        return statusMap.at(s);
    }

    void Player::checkGameIsActive() const {
        if (!game->isStarted() || game->isOver()) {
            throw std::runtime_error("The game is not active.");
        }
    }

    void Player::checkPlayerTurn() const {
        if (!game->isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn to play.");
        }
    }    
    
    void Player::gather() {
        checkGameIsActive();
        checkPlayerTurn();

        if(isStatusActive(Status::BlockedForGather)){
            throw runtime_error("You are blocked to perform this action.");
        }

        receiveCoins(1);
        game->logAction(name, ActionType::Gather, DeletableActionType::Gather);
        game->endTurn();
    }

    void Player::tax() {
        checkGameIsActive();
        checkPlayerTurn();

        if(isStatusActive(Status::BlockedForTax)){
            throw runtime_error("You are blocked to perform this action.");
        }

        receiveCoins(2); // Governor will override this to take 3
        game->logAction(name, ActionType::Tax, DeletableActionType::Tax);
        game->endTurn();
    }

    void Player::bribe() {
        checkGameIsActive();
        checkPlayerTurn();

        if(!hasEnoughCoins(4)){
            throw runtime_error("You don't have enough coins to perform a bribe.");
        }
        loseCoins(4);
        game->logAction(name, ActionType::Bribe, DeletableActionType::Bribe);
        setStatus(Status::HasExtraAction,true) ;
    }

    void Player::arrest(Player& toArrest) {
        checkGameIsActive();
        checkPlayerTurn();
        
        if (game->getLastArrested() == toArrest.getName()) {
            throw runtime_error("You cannot arrest the same player two turns in a row.");
        }
        if (!toArrest.hasEnoughCoins(1)) {
            throw runtime_error("The player does not have enough coins.");
        }

        toArrest.loseCoins(1);
        receiveCoins(1);
        game->logAction(name, ActionType::Arrest, DeletableActionType::None);
        game->setLastArrested(toArrest.getName());

        game->endTurn();
    }

    void Player::sanction(Player& toSanction) {
        checkGameIsActive();
        checkPlayerTurn();

        if (!hasEnoughCoins(3)) {
            throw runtime_error("You don't have enough coins to perform an arrest.");
        }
        toSanction.setStatus(Status::BlockedForGather,true);
        toSanction.setStatus(Status::BlockedForTax,true);

        loseCoins(3);
        game->logAction(name, ActionType::Sanction, DeletableActionType::None);

        game->endTurn();
    }

    void Player::coup(Player& toEliminate) {
        checkGameIsActive();
        checkPlayerTurn();

        if (!hasEnoughCoins(7)) {
            throw runtime_error("You don't have enough coins to perform a coup.");
        }
        loseCoins(7);
        toEliminate.eliminate();
        game->logAction(name, ActionType::Coup, DeletableActionType::None);


        game->endTurn();
    }

}
