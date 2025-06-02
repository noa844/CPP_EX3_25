#pragma once
#include <vector>

namespace coup {

    /**
     * @brief Represents internal player states or temporary conditions.
     */
    enum class Status {
        BlockedForTax,        ///< Player cannot perform Tax
        BlockedForGather,     ///< Player cannot perform Gather
        HasExtraAction,       ///< Player is allowed to perform an extra action this turn
        BlockedForArrest      ///< Player is blocked from arresting others
    };

    
}
