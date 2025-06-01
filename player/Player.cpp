#include "Player.hpp"
#include "../game_logic/Game.hpp"
#include "Status.hpp"
#include "../lib/magic_enum.hpp"
#include "../player/roles/Baron.hpp"
#include "../player/roles/General.hpp"
#include "../player/roles/Judge.hpp"
#include "../player/roles/Merchant.hpp"
#include <stdexcept>
#include <iostream>
using namespace std;
using coup::Status;
using namespace magic_enum;
using std::cout;
using std::endl;



namespace coup {

    Player::Player(const string& name, Game* game, RoleType roleType) : name(name),game(game), coinsCount(0),active(true),roleType(roleType) {
        if (name.length() > 9) {
            throw std::invalid_argument("Player name must not exceed 9 characters.");
        }
        initializeStatusMap();
        game->addPlayer(this);
    }

    Player::~Player(){ std::cout << "Destruct Player: " << name << std::endl;}


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

    RoleType Player::getRoleType() const {
        return roleType;
    }
    
    std::string Player::getRoleName() const {
        return to_string(roleType);
    }

    bool Player::isActive() const {
        return active;
    }

    void Player::eliminate() {
        active = false;
    }

    void Player::revive() {
        active = true;
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
    
    void Player::resetCoinsCount() {
        coinsCount = 0;
    }    


    bool Player::isStatusActive(Status s)const {
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

    bool Player::mustPerformCoup() const{
        if(coinsCount >= 10){
            return true;
        }
        else {
            return false;
        }
    }
    bool Player::lastActionIsBribe() const{
        if(isStatusActive(Status::HasExtraAction)){
            return true;
        }
        else{
            return false;
        }
    }
    bool Player::hasEnoughCoinForCoup(int coins) const{
        if((coinsCount - coins) >= 7){
            return true;
        }
        else {
            return false;
        }
    }

    std::vector<SpecialActionInfo> Player::getSpecialActions() {
       return {};
    } 

    void Player::executeSpecialAction(AllSpecialActionType, Player*) {
        throw std::runtime_error("Base Player cannot execute special actions.");
    }
    
    void Player::gather() {
        checkGameIsActive();
        checkPlayerTurn();

        if(mustPerformCoup()){
            throw runtime_error("You have to perform coup instead of this action.");
        }

        if(isStatusActive(Status::BlockedForGather)){
            throw runtime_error("You are blocked to perform gather.");
        }

        receiveCoins(1);
        game->logAction(name, ActionType::Gather, DeletableActionType::Gather);
        game->endTurn();
    }

    void Player::tax() {
        checkGameIsActive();
        checkPlayerTurn();

        if(mustPerformCoup()){
            throw runtime_error("You have to perform coup instead of this action.");
        }

        if(isStatusActive(Status::BlockedForTax)){
            throw runtime_error("You are blocked to perform tax.");
        }

        receiveCoins(2); // Governor will override this to take 3
        game->logAction(name, ActionType::Tax, DeletableActionType::Tax);
        game->endTurn();
    }

    void Player::bribe() {
        checkGameIsActive();
        checkPlayerTurn();

        if(mustPerformCoup()){
            throw runtime_error("You have to perform coup instead of this action.");
        }

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

        if(mustPerformCoup()){
            throw runtime_error("You have to perform coup instead of this action.");
        }

        if(isStatusActive(Status::BlockedForArrest)){
            throw runtime_error("You are blocked to perform an arrest.");
        }

        if (!toArrest.hasEnoughCoins(1)) {
            throw runtime_error("The target does not have enough coins.");
        }

        if(!toArrest.isActive() || toArrest.getName() == this->getName()){
            throw runtime_error("You cannot arrest yourself or an inactive player.");
        }
        
        if (game->getLastArrested() == toArrest.getName()) {
            throw runtime_error("You cannot arrest the same player two turns in a row.");
        }
        
        
        
        General* general = dynamic_cast<General*>(&toArrest);
        Merchant* merchant = dynamic_cast<Merchant*>(&toArrest);

        if (general) {
            //nothing
            
        } else if (merchant) {
           
            merchant->handleArrest();

        } else {
            
            toArrest.loseCoins(1);
            receiveCoins(1);
        }

        game->logAction(name, ActionType::Arrest, DeletableActionType::None,toArrest.getName());
        game->setLastArrested(toArrest.getName());

        game->endTurn();
    }

    void Player::sanction(Player& toSanction) {
        checkGameIsActive();
        checkPlayerTurn();

        if(mustPerformCoup()){
            throw runtime_error("You have to perform coup instead of this action.");
        }

        if (!hasEnoughCoins(3)) {
            throw runtime_error("You don't have enough coins to perform a sanction.");
        }

        if(!toSanction.isActive() || toSanction.getName() == this->getName()){
            throw runtime_error("You cannot sanction yourself or an inactive player.");
        }

        bool isJudge = dynamic_cast<Judge*>(&toSanction) != nullptr;
        bool isBaron = dynamic_cast<Baron*>(&toSanction) != nullptr;
        
        if (!hasEnoughCoins(4) && isJudge ) {
            throw runtime_error("You don't have enough coins to perform a sanction on Judge.");
        }

        toSanction.setStatus(Status::BlockedForGather, true);
        toSanction.setStatus(Status::BlockedForTax, true);

        if (isBaron) {
            toSanction.receiveCoins(1);  
        }

        if (isJudge) {
            this->loseCoins(1);  
        }


        loseCoins(3);
        game->logAction(name, ActionType::Sanction, DeletableActionType::None,toSanction.getName());

        game->endTurn();
    }

    void Player::coup(Player& toEliminate) {
        checkGameIsActive();
        checkPlayerTurn();
        
        if (!hasEnoughCoins(7)) {
            throw runtime_error("You don't have enough coins to perform a coup.");
        }

        if(!toEliminate.isActive() || toEliminate.getName() == this->getName()){
            throw runtime_error("You cannot eliminate yourself or an inactive player.");
        }
        
       
        loseCoins(7);
        
        game->logAction(name, ActionType::Coup, DeletableActionType::None,toEliminate.getName());
        game->eliminate(toEliminate);

        game->endTurn();
    }

}
