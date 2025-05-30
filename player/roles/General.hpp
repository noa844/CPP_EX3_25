#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"


namespace coup {

    class General : public Player {
    public:
        General(const std::string& name, Game* game);

        void blockCoup(Player& victim);
    };

}
