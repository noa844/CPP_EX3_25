#pragma once
#include "../player/Player.hpp"

namespace coup {

    class Judge : public Player {
    public:
        Judge(const std::string& name, Game* game);

        void deleteBribe(Player& target); 
    };

}
