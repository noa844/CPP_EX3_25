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
        initializeStatusMap();
        game->addPlayer(this);
    }

    Player::~Player()= default;


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
            throw runtime_error("You are blocked to perform tax.");
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
        std::cout << "[Arrest] " << name << " is arresting " << toArrest.getName() << "\n";
        std::cout << "[Arrest] Target coins before: " << toArrest.getCoinsCount() << "\n";
    
        
        if (game->getLastArrested() == toArrest.getName()) {
            throw runtime_error("You cannot arrest the same player two turns in a row.");
        }
        if (!toArrest.hasEnoughCoins(1)) {
            throw runtime_error("The target does not have enough coins.");
        }
        if(isStatusActive(Status::BlockedForArrest)){
            throw runtime_error("You are blocked to perform an arrest.");
        }

        
        
        General* general = dynamic_cast<General*>(&toArrest);
        Merchant* merchant = dynamic_cast<Merchant*>(&toArrest);

        std::cout << "dynamic_cast General = " << (dynamic_cast<General*>(&toArrest) != nullptr) << std::endl;
        std::cout << "dynamic_cast Merchant = " << (dynamic_cast<Merchant*>(&toArrest) != nullptr) << std::endl;

        if (general) {
            //nothing
            std::cout << "[Arrest] Target is General\n";
        } else if (merchant) {
            std::cout << "[Arrest] Target is Merchant\n";
            merchant->handleArrest();

        } else {
            std::cout << "[Arrest] Target is regular player\n";
            toArrest.loseCoins(1);
            receiveCoins(1);
        }

       
        std::cout << "[Arrest] Target coins after: " << toArrest.getCoinsCount() << "\n";
        std::cout << "[Arrest] Arrest finished\n";
        game->logAction(name, ActionType::Arrest, DeletableActionType::None,toArrest.getName());
        game->setLastArrested(toArrest.getName());

        game->endTurn();
    }

    void Player::sanction(Player& toSanction) {
        checkGameIsActive();
        checkPlayerTurn();

        
        bool isJudge = dynamic_cast<Judge*>(&toSanction) != nullptr;
        bool isBaron = dynamic_cast<Baron*>(&toSanction) != nullptr;

        
        int requiredCoins = isJudge ? 4 : 3;
        if (!hasEnoughCoins(requiredCoins)) {
            throw runtime_error(isJudge 
        ? "You don't have enough coins to perform a sanction on Judge." 
        : "You don't have enough coins to perform a sanction." );
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
        loseCoins(7);
        
        game->logAction(name, ActionType::Coup, DeletableActionType::None,toEliminate.getName());
        game->eliminate(toEliminate);

        game->endTurn();
    }

}
