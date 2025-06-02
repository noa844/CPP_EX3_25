#include "RoleType.hpp"
#include "SpecialActionUtils.hpp"

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

    std::string toString(AllSpecialActionType role) {
        switch (role) {
            case AllSpecialActionType::Peek_Coins: return "Peek Coins";
            case AllSpecialActionType::Delete_bribe: return "Delete bribe";
            case AllSpecialActionType::Delete_coup: return "Delete coup";
            case AllSpecialActionType::Delete_tax: return "Delete tax";
            case AllSpecialActionType::Invest: return "Invest";
            case AllSpecialActionType::Block_Arrest: return "Block Arrest";
            default: return "None";
        }
    }


}
