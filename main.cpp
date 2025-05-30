#include "../game_logic/Game.hpp"
#include "../GUI/GameGui.hpp"
using namespace coup;
int main() {

    // Création du jeu
    Game game;

    // Création de l'interface graphique SFML
    GameGui gui(game);

    // Lancement de la boucle SFML
    gui.run();

    return 0;
}

