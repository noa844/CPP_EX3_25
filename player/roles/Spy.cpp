#include "Spy.hpp"
#include "../game_logic/Game.hpp"

namespace coup {

    Spy::Spy(const std::string& name, Game* game)
        : Player(name, game) {}

    int Spy::peekCoinsCount(const Player& target) const {
        checkGameIsActive();
        return target.getCoinsCount();
    }

    void Spy::blockArrest(Player& target) {
        checkGameIsActive();
        target.setStatus(Status::BlockedForArrest, true);
    }
}