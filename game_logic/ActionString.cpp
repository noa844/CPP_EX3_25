#include "ActionType.hpp"


namespace coup {

    std::string toString(ActionType action) {
    switch (action) {
        case ActionType::Tax: return "Tax";
        case ActionType::Gather: return "Gather";
        case ActionType::Bribe: return "Bribe";
        case ActionType::Arrest: return "Arrest";
        case ActionType::Sanction: return "Sanction";
        case ActionType::Coup: return "Coup";
        default: return "None";
    }
}   
}