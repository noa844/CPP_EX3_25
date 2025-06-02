#include "../game_logic/Game.hpp"
#include "../GUI/GameGui.hpp"


using namespace coup;
int main() {
    Game game;

    GameGui gui(game);

    gui.run();

    return 0;


}

