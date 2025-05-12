#pragma once
#include <string>
using std::string;

namespace coup {

    class Player {
    protected:
        string name;
        int coinsCount = 0;
    
    public:
        //constructor
        Player(const string& name);

        //destructor
        virtual ~Player() = default;
        
        //coins acction
        void receiveCoins(int amount);
        void loseCoins(int amount);
        int getCoinsCount() const;

        string getName() const;
    };
}
    