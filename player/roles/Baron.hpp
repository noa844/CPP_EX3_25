#pragma once
#include "../player/Player.hpp"
#include "../player/RoleType.hpp"

namespace coup {

    class Baron : public Player {
    public:
        Baron(const std::string& name, Game* game);

        void invest(); // Pay 3 to get 6
        
    };

}