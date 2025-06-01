#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../game_logic/Game.hpp" 
#include "../player/Player.hpp"
#include "../game_logic/ActionType.hpp" 


namespace coup {

    struct SpecialActionButton {
        sf::RectangleShape shape;
        sf::Text label;
        std::string playerName;
        AllSpecialActionType actionType;
    };

    class GameGui {
    private:
        Game& game; 
        sf::Font font;
        std::string currentName;
        std::vector<Player*> guiPlayers;
        std::vector<std::string> activeNames;
        
        

        // composants graphiques
        sf::RenderWindow window;
        sf::Text inputText;
        sf::Text instruction;
        sf::RectangleShape startButton;
        sf::Text startText;
        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;


        std::vector<sf::RectangleShape> actionButtons;
        std::vector<sf::Text> actionLabels;
        std::vector<ActionType> actionTypes;
        std::vector<sf::RectangleShape> targetButtons;
        std::vector<sf::Text> targetLabels;
        std::vector<SpecialActionButton> specialActionButtons;
        bool awaitingTargetSelection = false;

        ActionType selectedAction = ActionType::None;
        std::string actionErrorMessage;
        std::string infoMessage;
        // Pour stocker une action spéciale sélectionnée qui nécessite une cible
        coup::SpecialActionInfo selectedSpecialAction;
        coup::Player* specialActionOwner = nullptr;



        void setupGUI();
        void handleEvents();
        void executeSelectedAction();
        void executeSelectedAction(Player& target);
        void render();
        void displayGameState();
        void displayActionButtons();
        void displayTargetSelection();
        void clearTargetSelection();

        


    public:
        GameGui(Game& game);
        ~GameGui();
        void run();
    };

}
