#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../game_logic/Game.hpp" 
#include "../player/Player.hpp"

namespace coup {

    class GameGui {
    private:
        Game& game; // référence à ton moteur de jeu
        sf::Font font;
        std::string currentName;
        std::vector<Player*> guiPlayers;
        bool gameStarted = false;
        size_t currentPlayerIndex = 0;

        // composants graphiques
        sf::RenderWindow window;
        sf::Text inputText;
        sf::Text instruction;
        sf::RectangleShape startButton;
        sf::Text startText;

        void setupGUI();
        void handleEvents();
        void render();
        void displayGameState();

    public:
        GameGui(Game& game);
        void run();
    };

}
