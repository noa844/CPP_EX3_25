#include "Spy.hpp"
#include "../game_logic/Game.hpp"

namespace coup {

    Spy::Spy(const std::string& name, Game* game, RoleType role)
    : Player(name, game, role) {}

    int Spy::peekCoinsCount(const Player& target) const {
        checkGameIsActive();
        return target.getCoinsCount();
    }

    void Spy::blockArrest(Player& target) {
        checkGameIsActive();
        target.setStatus(Status::BlockedForArrest, true);
    }
}