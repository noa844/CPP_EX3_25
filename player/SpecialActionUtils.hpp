#pragma once
#include "RoleType.hpp"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <string>
#include "../lib/magic_enum.hpp"



namespace coup {
    
    enum class AllSpecialActionType {
        Peek_Coins,
        Block_Arrest,
        Delete_bribe,
        Delete_tax,
        Delete_coup,
        Invest
    };


    struct SpecialActionInfo {

        AllSpecialActionType name;
        bool requiresTarget;
        bool canBeUsedAnytime;
    };    

    template<typename Enum>
    std::vector<std::string> getSpecialActionNames() {
        std::vector<std::string> names;
        for (auto val : magic_enum::enum_values<Enum>()) {
            names.push_back(std::string(magic_enum::enum_name(val)));
        }
        return names;
    }

   

}
