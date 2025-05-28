#pragma once
#include "../player/Player.hpp"

namespace coup {

    class Spy : public Player {

    public:
        Spy(const std::string& name, Game* game);

        int peekCoinsCount(const Player& target) const;
        void blockArrest(Player& target);    

    };

}
