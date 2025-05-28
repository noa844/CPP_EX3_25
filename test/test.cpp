#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../player/Player.hpp"
#include "../game_logic/Game.hpp"
#include "../player/Status.hpp"
#include "../player/magic_enum.hpp"
#include <stdexcept>
using namespace coup;
using coup::Game;

TEST_CASE("Player and Game functionality with game start/end checks") {
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

    SUBCASE("Sanction disables target's gather and tax") {
        alice->receiveCoins(3);
        alice->sanction(*bob);
        CHECK(bob->isStatusActive(Status::BlockedForGather));
        CHECK(bob->isStatusActive(Status::BlockedForTax));
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

        delete p;
    }

