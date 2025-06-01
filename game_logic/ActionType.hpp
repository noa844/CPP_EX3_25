#pragma once
#include <string>

namespace coup {

    /**
     * @brief Represents the list of all possible game actions.
     * 
     * This enumeration defines all types of actions that can be performed by players during the game.
     */
    enum class ActionType {
        Tax,        ///< Player collects 3 coins from the bank.
        Gather,     ///< Player collects 1 coin.
        Bribe,      ///< Player gives coins to another player to gain favor or avoid arrest.
        Arrest,     ///< Player attempts to arrest another player.
        Sanction,   ///< Player applies a penalty to another player.
        Coup,       ///< Player eliminates another player at the cost of 7 coins.
        None        ///< Placeholder for no action or uninitialized state.
    };

    /**
     * @brief Converts an ActionType to its string representation.
     * 
     * @param action The ActionType to convert.
     * @return A string representation of the action (e.g., "Tax", "Coup").
     */
    std::string toString(ActionType action);

}
