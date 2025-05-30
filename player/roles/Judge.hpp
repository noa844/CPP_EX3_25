#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"

namespace coup {

    class Judge : public Player {
    public:
        Judge(const std::string& name, Game* game);

        void deleteBribe(Player& target); 
    };

}
