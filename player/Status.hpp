#pragma once
#include <vector>

namespace coup {

    enum class Status {
        BlockedForTax,
        BlockedForGather,
        WasArrestedLastTurn,
        HasExtraAction
    };
    
    // Utility function to get all possible statuses
    inline std::vector<Status> getAllStatuses() {
        return {
            Status::BlockedForTax,
            Status::BlockedForGather,
            Status::WasArrestedLastTurn,
            Status::HasExtraAction
        };
    }
}
