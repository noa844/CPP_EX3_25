#pragma once
#include "../player/Player.hpp"

namespace coup {

    class Governor : public Player {
    public:
        Governor(const std::string& name, Game* game);

        // Override tax behavior
        void tax() override;
        void deleteTax(Player& target);
    };

}
