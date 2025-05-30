#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"

namespace coup {

    class Spy : public Player {

    public:
        Spy(const std::string& name, Game* game, RoleType role);

        int peekCoinsCount(const Player& target) const;
        void blockArrest(Player& target);    

    };

}
