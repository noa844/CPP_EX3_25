#include "PlayerFactory.hpp"
#include "RoleType.hpp"
#include "../roles/Spy.hpp"
#include "../roles/Baron.hpp"
#include "../roles/General.hpp"
#include "../roles/Governor.hpp"
#include "../roles/Judge.hpp"
#include "../roles/Merchant.hpp"
#include <random>
#include <vector>
#include <stdexcept>


namespace coup {

    RoleType getRandomRoleType() {
        static std::vector<RoleType> allRoles = {
            RoleType::Spy,
            RoleType::Baron,
            RoleType::General,
            RoleType::Governor,
            RoleType::Judge,
            RoleType::Merchant,
        };

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, allRoles.size() - 1);

        return allRoles[dist(gen)];
    }

    Player* PlayerFactory::createPlayer(const std::string& name, Game* game) {
    
        RoleType role = getRandomRoleType();
        switch (role) {
            case RoleType::Governor:
             return new Governor(name, game,role);
           
            case RoleType::Spy: 
              return new Spy(name, game,role);
            
            case RoleType::Baron:
              return new Baron(name, game,role);
             
            case RoleType::Merchant:
                  return new Merchant(name, game,role);
            
            case RoleType::General:
              return new General(name, game,role);
             
            case RoleType::Judge:
             return new Judge(name, game,role);

            default:
             throw std::invalid_argument("Unknown role type in factory");
        }
    
    }

}
