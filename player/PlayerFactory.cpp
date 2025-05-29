#include "PlayerFactory.hpp"

namespace coup {

    Player* PlayerFactory::createPlayer(const std::string& name, Game* game) {
    
        int index = std::rand() % 6;

        Player* player = nullptr;
        switch (index) {
            case 0: player = new Governor(name, game); break;
            case 1: player = new Spy(name, game); break;
            case 2: player = new Baron(name, game); break;
            case 3: player = new Merchant(name, game); break;
            case 4: player = new General(name, game); break;
            case 5: player = new Judge(name, game); break;
        }

    
        return player;
    }

}
