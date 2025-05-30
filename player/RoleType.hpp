#pragma once
#include <string>

namespace coup {

    enum class RoleType {
        Spy,
        Baron,
        General,
        Governor,
        Judge,
        Merchant,
        None
    };

    
    std::string to_string(RoleType role);

}
