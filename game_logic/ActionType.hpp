#pragma once
#include <string>
namespace coup {

    enum class ActionType {
        Tax,
        Gather,
        Bribe,
        Arrest,
        Sanction,
        Coup,
        None
    };

    std::string toString(ActionType action);    

}
