#pragma once

namespace coup {

    /**
     * @brief Represents actions that can be deleted or cancelled by other roles.
     * 
     * Some roles in the game can cancel specific actions (e.g., Governor cancels Tax).
     * This enumeration lists only those actions that are eligible for cancellation.
     */
    enum class DeletableActionType {
        Tax,       ///< The Tax action can be deleted.
        Gather,    ///< The Gather action can be deleted.
        Bribe,     ///< The Bribe action can be deleted.
        None       ///< Placeholder for no deletable action.
    };

}

