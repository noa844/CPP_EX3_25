#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../player/Player.hpp"
#include "../game_logic/Game.hpp"
#include "../player/Status.hpp"
#include "../lib/magic_enum.hpp"
#include "../player/roles/Governor.hpp"
#include "../player/roles/Spy.hpp"
#include "../player/roles/Baron.hpp"
#include "../player/roles/General.hpp"
#include "../player/roles/Judge.hpp"
#include "../player/roles/Merchant.hpp"
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
        CHECK_THROWS_WITH(alice->tax(), "You are blocked to perform tax.");
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
        CHECK_THROWS_WITH(alice->arrest(*bob), "The target does not have enough coins.");
    }

    SUBCASE("Sanction disables target's gather and tax for one turn") {
        alice->receiveCoins(4);
        bob->receiveCoins(4);
        alice->sanction(*bob);
        CHECK(bob->isStatusActive(Status::BlockedForGather));
        CHECK(bob->isStatusActive(Status::BlockedForTax));
        bob->bribe();
        bob->arrest(*alice);
        CHECK_THROWS_WITH(bob->tax();, "You are blocked to perform tax.");
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
    TEST_CASE("Test Spy behavior") {
        coup::Game game3;
    
        Spy* spy = new Spy("Spy", &game3);
        Player* bob = new Player("Bob", &game3);
    
        game3.start();
    
        SUBCASE("Spy see Bob's coins count (not considered as a turn)") {
            bob->receiveCoins(3);
            int count = spy->peekCoinsCount(*bob);
            CHECK(count == 3);
            spy->tax();
        }
        SUBCASE("Spy block Bob's to perform arrest") {
            spy->blockArrest(*bob);
            spy->tax();
            CHECK_THROWS_WITH(bob->arrest(*spy),"You are blocked to perform an arrest.");
            
            bob->tax();
            game3.nextTurn();//skip spy's turn
            bob->receiveCoins(5);
            bob->arrest(*spy);//now bob can arrest spy
        }
    }
        TEST_CASE("Test Baron behavior") {
            coup::Game game4;
        
            Baron* baron = new Baron("Baron", &game4);
            Player* bob = new Player("Bob", &game4);
        
            game4.start();
        
            SUBCASE("Baron invest") {
               
               CHECK_THROWS_WITH(baron->invest();,"Not enough coins to invest.");
               baron->receiveCoins(3);
               baron->invest();
               CHECK(baron->getCoinsCount()==6);

            }
            SUBCASE("Baron sanctioned get 1 coin") {
                baron->tax();
                bob->receiveCoins(3);
                bob->sanction(*baron);
                CHECK_THROWS_WITH(baron->tax(),"You are blocked to perform tax.");
                CHECK(baron->getCoinsCount()==3);
                game4.nextTurn();//skip baron's turn
                game4.nextTurn();//skip bob's turn
                baron->gather();
                CHECK(baron->getCoinsCount()==4);
            }
    }



    TEST_CASE("Test General behavior") {
        Game game5;
        
        Player* alice = new Player("Alice", &game5);
        Player* bob = new Player("Bob", &game5);
        General* general = new General("General", &game5);

        game5.start();

        SUBCASE("General blocks a recent coup"){
            // Setup: Alice has 7 coins to perform a coup
            alice->receiveCoins(7);
            alice->coup(*bob);
            CHECK_FALSE(bob->isActive());

            // General gets 5 coins to be able to block the coup
            general->receiveCoins(5);
            general->blockCoup(*bob);
            CHECK(bob->isActive());       // Bob is revived
            CHECK(general->getCoinsCount() == 0);  // General paid 5
            CHECK(alice->getCoinsCount() == 0);  // alice lose 7
            general->tax();

            //check if turns order is same
            CHECK(game5.isPlayerTurn(alice));
            game5.nextTurn();
            CHECK(game5.isPlayerTurn(bob));
            game5.nextTurn();
            CHECK(game5.isPlayerTurn(general));
            

        }
        SUBCASE("General blocks a recent coup not immediately"){
            Player* clara = new Player("Clara", &game5);
            //round 1
            game5.nextTurn();//skip alice
            game5.nextTurn();//skip bob
            game5.nextTurn();//skip general
            clara->receiveCoins(7);
            clara->coup(*alice);
            CHECK_FALSE(alice->isActive());
            //round 2
            game5.nextTurn();//skip bob
            // General gets 5 coins to be able to block the coup
            general->receiveCoins(5);
            general->blockCoup(*alice);
            CHECK(alice->isActive());
            game5.nextTurn();//skip general
            game5.nextTurn();//skip clara
            //round 3
            CHECK(game5.isPlayerTurn(alice));

        }

        SUBCASE("General fails to block old coup") {
           //round 1
            alice->receiveCoins(7);
            alice->coup(*bob);
            CHECK_FALSE(bob->isActive());

            game5.nextTurn(); // General didn't block, now it's new round
            //round 2
            alice->tax(); 
            general->receiveCoins(5);
            CHECK_THROWS_WITH(general->blockCoup(*bob), "No recent coup on this player to block.");
        }
        SUBCASE("General arrested get his coin back") {
            general->receiveCoins(1);
            alice->arrest(*general);
            CHECK(general->getCoinsCount()==1);
            CHECK(alice->getCoinsCount()==0);
        }
    }

    TEST_CASE("Test Judge behavior") {
        Game game6;
        Player* actor = new Player("Actor", &game6);
        Player* normal = new Player("Normal", &game6);
        Baron* baron = new Baron("Baron", &game6);
        Judge* judge = new Judge("Judge", &game6);
    
        game6.start();
    
        SUBCASE("Sanction baron gives him 1 coin") {
            actor->receiveCoins(3);
            int initial = baron->getCoinsCount();
            actor->sanction(*baron);
            CHECK(baron->getCoinsCount() == initial + 1);
            CHECK(baron->isStatusActive(Status::BlockedForTax));
            CHECK(baron->isStatusActive(Status::BlockedForGather));
        }
    
        SUBCASE("Sanction judge costs 4 coins") {
            actor->receiveCoins(4);
            int before = actor->getCoinsCount();
            actor->sanction(*judge);
            CHECK(judge->isStatusActive(Status::BlockedForTax));
            CHECK(judge->isStatusActive(Status::BlockedForGather));
            CHECK(actor->getCoinsCount() == before - 4);
        }
    
        SUBCASE("Fail if not enough coins for judge") {
            actor->receiveCoins(3);
            CHECK_THROWS_WITH(actor->sanction(*judge), "You don't have enough coins to perform a sanction on Judge.");
        }
    
        SUBCASE("Fail if not enough coins for regular sanction") {
            CHECK_THROWS_WITH(actor->sanction(*normal), "You don't have enough coins to perform a sanction.");
        }

        SUBCASE("Judge delete bribe") {
            actor->receiveCoins(4);
            actor->bribe();
            CHECK(actor->isStatusActive(Status::HasExtraAction));
            judge->deleteBribe(*actor);
            CHECK_FALSE(actor->isStatusActive(Status::HasExtraAction));
        }
        SUBCASE("Judge delete bribe with no bribe to delete") {
            actor->receiveCoins(4);
            actor->tax();
            CHECK_THROWS_WITH(judge->deleteBribe(*actor),"No recent bribe to block for this player.");  
        }
        SUBCASE("Judge delete bribe to late") {
            actor->receiveCoins(4);
            actor->bribe();
            actor->tax();
            actor->tax();
            CHECK_THROWS_WITH(judge->deleteBribe(*actor),"Cannot cancel bribe: player already used bonus action.");  
        }
    }
   
    TEST_CASE("Test Merchant behavior") {
        Game game6;

        Player* normal = new Player("Normal", &game6);
        Merchant* merchant = new Merchant("Merchant", &game6);


        game6.start();

        SUBCASE("Merchant gets 1 extra coin at start of turn if has >= 3 coins") {
            merchant->receiveCoins(3);
            CHECK(merchant->getCoinsCount() == 3);

            game6.nextTurn();  // skip normal’s turn
            CHECK(game6.isPlayerTurn(merchant));
            CHECK(merchant->getCoinsCount() == 4);  // Received bonus
        }

        SUBCASE("Merchant does not get extra coin if has < 3 coins") {
            merchant->receiveCoins(2);
            CHECK(merchant->getCoinsCount() == 2);

            game6.nextTurn();  // Go to Merchant’s turn
            CHECK(game6.isPlayerTurn(merchant));
            CHECK(merchant->getCoinsCount() == 2);  // No bonus
        }

        SUBCASE("Merchant loses 2 coins to bank when arrested") {
            merchant->receiveCoins(2);
            CHECK(merchant->getCoinsCount() == 2);

            // simulate arrest from normal player
            normal->arrest(*merchant);

            CHECK(merchant->getCoinsCount() == 0);  // Lost 2
            CHECK(normal->getCoinsCount() == 0);    // Did NOT gain anything
        }

        SUBCASE("Merchant loses only 1 coin if has 1") {

            merchant->receiveCoins(1);
            normal->arrest(*merchant);
            

            CHECK(merchant->getCoinsCount() == 0);
            CHECK(normal->getCoinsCount() == 0);  // Always zero for normal
        }

        SUBCASE("Merchant loses 0 if has 0") {
            CHECK_THROWS_WITH(normal->arrest(*merchant);,"The target does not have enough coins.");
            CHECK(normal->getCoinsCount() == 0);
        }
    }

            