#include "Player.hpp"
#include "Game.hpp"
#include "Status.hpp"
#include <stdexcept>
using namespace std;
using coup::Status;




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

        for (Status s : getAllStatuses()) {
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

    void Player::gather() {
        if(isStatusActive(Status::BlockedForGather)){
            throw runtime_error("You are blocked to perform this action.");
        }

        receiveCoins(1);

        game->endTurn();
    }

    void Player::tax() {
        if(isStatusActive(Status::BlockedForTax)){
            throw runtime_error("You are blocked to perform this action.");
        }

        receiveCoins(2); // Governor will override this to take 3

        game->endTurn();
    }

    void Player::bribe() {
        if(!hasEnoughCoins(4)){
            throw runtime_error("You don't have enough coins to perform a bribe.");
        }
        loseCoins(4);
        setStatus(Status::HasExtraAction,true) ;
    }

    void Player::arrest(Player& toArrest) {
        if (toArrest.isStatusActive(Status::WasArrestedLastTurn)) {
            throw runtime_error("You cannot arrest the same player two turns in a row.");
        }
        if (!toArrest.hasEnoughCoins(1)) {
            throw runtime_error("The player does not have enough coins.");
        }

        toArrest.loseCoins(1);
        receiveCoins(1);
    
        setStatus(Status::WasArrestedLastTurn,true) ;

        game->endTurn();
    }

    void Player::sanction(Player& toSanction) {
        if (!hasEnoughCoins(3)) {
            throw runtime_error("You don't have enough coins to perform an arrest.");
        }
        toSanction.setStatus(Status::BlockedForGather,true);
        toSanction.setStatus(Status::BlockedForTax,true);

        loseCoins(3);
        game->endTurn();
    }

    void Player::coup(Player& toEliminate) {
        if (!hasEnoughCoins(7)) {
            throw runtime_error("You don't have enough coins to perform a coup.");
        }
        loseCoins(7);
        toEliminate.eliminate();

        game->endTurn();
    }

}
