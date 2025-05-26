#include "doctest.h"
#include "Player.hpp"
#include "Game.hpp"
#include "Status.hpp"
#include <stdexcept>
using namespace coup;
using coup::Game;


TEST_CASE("Player and Game basic functionality") {

    Game game_1;

    Player* alice = new Player("Alice", &game_1);
    Player* bob = new Player("Bob", &game_1);

    CHECK(alice->getName() == "Alice");
    CHECK(bob->getCoinsCount() == 0);
    CHECK(game_1.currentPlayer().getName() == "Alice");

    SUBCASE("Gather should add one coin and move to next player") {
        alice->gather();
        CHECK(alice->getCoinsCount() == 1);
        CHECK(game_1.currentPlayer().getName() == "Bob");
    }

    SUBCASE("BlockedForGather prevents gather") {
        alice->setStatus(Status::BlockedForGather, true);
        CHECK_THROWS_WITH(alice->gather(), "You are blocked to perform this action.");
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
        CHECK(alice->isStatusActive(Status::WasArrestedLastTurn));
    }

    SUBCASE("Arrest fails if target was arrested last turn") {
        bob->setStatus(Status::WasArrestedLastTurn, true);
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
    }

    SUBCASE("Coup fails without enough coins") {
        CHECK_THROWS_WITH(alice->coup(*bob), "You don't have enough coins to perform a coup.");
    }

    delete alice;
    delete bob;
}
