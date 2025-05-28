#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../player/Player.hpp"
#include "../game_logic/Game.hpp"
#include "../player/Status.hpp"
#include "../lib/magic_enum.hpp"
#include "../player/roles/Governor.hpp"
#include "../player/roles/Spy.hpp"
#include <stdexcept>
using namespace coup;
using coup::Game;


TEST_CASE("Deque action history behavior") {
    Game game;
    Player* a = new Player("A", &game);
    Player* b = new Player("B", &game);
    Player* c = new Player("C", &game);


    game.start();

    SUBCASE("Action history size never exceeds activePlayers().size()") {
        a->receiveCoins(4); //to perform bribe
        //actions 
        a->bribe();
        a->tax();
        a->tax();
        b->tax();
        c->tax();
        CHECK(game.getActionHistorySize()==5);
        // now add "A" do one more action — should remove oldest A's actions
        a->tax();
        CHECK(game.getActionHistorySize()==3);

        //remove player
        b->receiveCoins(6);
        b->coup(*a);
        CHECK(game.getActionHistorySize() == 2);

        //Verify that second A's action was pushed out (because a is now eliminate)
        CHECK_FALSE(game.hasRecentDeletableAction("A",DeletableActionType::Tax));

    }


}


TEST_CASE("Player(without role) and Game functionality with game start/end checks") {
    Game game1;

    Player* alice = new Player("Alice", &game1);
   
    SUBCASE("Game must have at least two players to start") { 
        CHECK_THROWS_WITH(game1.start(), "The game requires at least 2 players to start.");
    }

    Player* bob = new Player("Bob", &game1);
    game1.start();
    CHECK(game1.isStarted());
    CHECK(alice->getName() == "Alice");
    CHECK(bob->getCoinsCount() == 0);
    CHECK(game1.currentPlayer().getName() == "Alice");

    SUBCASE("Only current player can act") {

    CHECK_THROWS_WITH(bob->gather(), "It's not your turn to play.");
    CHECK_NOTHROW(alice->gather());

    }

    SUBCASE("Gather should add one coin and move to next player") {
        alice->gather();
        CHECK(alice->getCoinsCount() == 1);
        CHECK(game1.currentPlayer().getName() == "Bob");
    }

    SUBCASE("BlockedForGather prevents gather") {
        game1.nextTurn();//skip Alice's turn.
        bob->receiveCoins(3);
        bob->sanction(*alice);
        CHECK_THROWS_WITH(alice->gather(), "You are blocked to perform this action.");
        //Alice can perform gather next turn;
        game1.nextTurn();//skip Alice's turn.
        game1.nextTurn();//skip Bob's turn.
        alice->gather();
        CHECK(alice->getCoinsCount() == 1);
    }

    SUBCASE("Tax should give 2 coins") {
        alice->tax();
        CHECK(alice->getCoinsCount() == 2);
    }

    SUBCASE("BlockedForTax prevents tax") {
        alice->setStatus(Status::BlockedForTax, true);
        CHECK_THROWS_WITH(alice->tax(), "You are blocked to perform this action.");
    }

    SUBCASE("Bribe consumes 4 coins and allows extra action") {
        alice->receiveCoins(4);
        alice->bribe();
        CHECK(alice->isStatusActive(Status::HasExtraAction));
        alice->tax();
        alice->tax();
        
    }

    SUBCASE("Arrest works and sets status") {
        bob->receiveCoins(1);
        alice->arrest(*bob);
        CHECK(alice->getCoinsCount() == 1);
        CHECK(bob->getCoinsCount() == 0);
        CHECK(game1.getLastArrested() == bob->getName());
        //Arrest fails if target was arrested last turn
        game1.nextTurn();
        CHECK_THROWS_WITH(alice->arrest(*bob), "You cannot arrest the same player two turns in a row.");
    }

   

    SUBCASE("Arrest fails if target has no coins") {
        CHECK_THROWS_WITH(alice->arrest(*bob), "The player does not have enough coins.");
    }

    SUBCASE("Sanction disables target's gather and tax for one turn") {
        alice->receiveCoins(4);
        bob->receiveCoins(4);
        alice->sanction(*bob);
        CHECK(bob->isStatusActive(Status::BlockedForGather));
        CHECK(bob->isStatusActive(Status::BlockedForTax));
        bob->bribe();
        bob->arrest(*alice);
        CHECK_THROWS_WITH(bob->tax();, "You are blocked to perform this action.");
        bob->receiveCoins(3);
        bob->sanction(*alice);
        game1.nextTurn();//skip Alice's turn
        //now bob can perform tax and gather
        bob->receiveCoins(3);
        bob->bribe();
        bob->tax();
        bob->gather();

        
    }

    SUBCASE("Coup eliminates target if enough coins") {
        alice->receiveCoins(7);
        alice->coup(*bob);
        CHECK(bob->isActive() == false);
        CHECK(game1.isOver());
    }

    SUBCASE("Coup fails without enough coins") {
        CHECK_THROWS_WITH(alice->coup(*bob), "You don't have enough coins to perform a coup.");
    }

}



    TEST_CASE("All statuses are initialized to false") {
        Game game;
        Player* p = new Player("Test", &game);

        for (Status s : magic_enum::enum_values<Status>()) {
            CHECK(p->isStatusActive(s) == false);
        }

    }


    TEST_CASE("Test Governor behavior") {
        coup::Game game2;
    
        Governor* gov = new Governor("Governor", &game2);
        Player* bob = new Player("Bob", &game2);
    
        game2.start();
    
        SUBCASE("Governor tax gives 3 coins") {
            gov->tax();
            CHECK(gov->getCoinsCount() == 3);
        }
    
        SUBCASE("Governor deletes Bob's tax") {
            
            game2.nextTurn(); // skip gov's turn.
            bob->tax();
            CHECK(bob->getCoinsCount() == 2);
    
            CHECK(game2.hasRecentDeletableAction("Bob", coup::DeletableActionType::Tax));
            gov->deleteTax(*bob);
            CHECK(bob->getCoinsCount() == 0); 
    
            CHECK_FALSE(game2.hasRecentDeletableAction("Bob", coup::DeletableActionType::Tax));
            CHECK(game2.hasRecentDeletableAction("Bob", coup::DeletableActionType::None));
        }
    }
    TEST_CASE("Test Governor behavior") {
        coup::Game game3;
    
        Spy* spy = new Spy("Spy", &game3);
        Player* bob = new Player("Bob", &game3);
    
        game3.start();
    
        SUBCASE("Spy see Bob's coins count (not considered as a turn)") {
            bob->receiveCoins(3);
            int count = spy->peekCoinsCount(*bob);
            CHECK(count == 3);
        }
    }
    