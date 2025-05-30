#include "Baron.hpp"
#include "../game_logic/Game.hpp"
#include <stdexcept>
#include "RoleType.hpp"

namespace coup {

    Baron::Baron(const std::string& name, Game* game, RoleType role)
        : Player(name, game, role) {}

    void Baron::invest() {
        checkGameIsActive();

        if (!hasEnoughCoins(3)) {
            throw std::runtime_error("Not enough coins to invest.");
        }

        loseCoins(3);
        receiveCoins(6);

    }
}