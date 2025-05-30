#include "RoleType.hpp"

namespace coup {

    std::string to_string(RoleType role) {
        switch (role) {
            case RoleType::Spy: return "Spy";
            case RoleType::Baron: return "Baron";
            case RoleType::General: return "General";
            case RoleType::Governor: return "Governor";
            case RoleType::Judge: return "Judge";
            case RoleType::Merchant: return "Merchant";
            default: return "None";
        }
    }

}
