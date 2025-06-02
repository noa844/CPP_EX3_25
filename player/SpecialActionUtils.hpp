#pragma once
#include "RoleType.hpp"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <string>
#include "../lib/magic_enum.hpp"



namespace coup {
    
    /**
     * @brief Enumerates all special actions available in the game.
     */
    enum class AllSpecialActionType {
        Peek_Coins,     ///< Spy: Peek at another player's coins
        Block_Arrest,   ///< Spy: Block an arrest
        Delete_bribe,   ///< Judge: Cancel a bribe action
        Delete_tax,     ///< Governor: Cancel a tax action
        Delete_coup,    ///< General: Cancel a coup
        Invest          ///< Baron: Pay 3 to gain 6
    };

    /**
     * @brief Converts an ActionType to its string representation.
     * 
     * @param action The ActionType to convert.
     * @return A string representation of the action (e.g., "Tax", "Coup").
     */
    std::string toString(AllSpecialActionType action);

    /**
     * @brief Holds metadata about a special action.
     */
    struct SpecialActionInfo {
        AllSpecialActionType name;    ///< Type of the special action
        bool requiresTarget;          ///< Whether the action needs a target
        bool canBeUsedAnytime;        ///< Whether it can be used outside the player's turn
    };

    /**
     * @brief Retrieves the list of string names for a given enum using magic_enum.
     * 
     * @tparam Enum The enum type to extract string names from.
     * @return A vector of string representations of enum values.
     */
    template<typename Enum>
    std::vector<std::string> getSpecialActionNames() {
        std::vector<std::string> names;
        for (auto val : magic_enum::enum_values<Enum>()) {
            names.push_back(std::string(magic_enum::enum_name(val)));
        }
        return names;
    }


}
