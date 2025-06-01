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

    /**
 * @brief Graphical User Interface (GUI) for the Coup game using SFML.
 * 
 * This class manages the entire game interface, from player creation to in-game interactions.
 * It allows players to be added via a text input and a button, displays their roles and actions, 
 * and supports executing both regular and special actions via on-screen buttons.
 */
class GameGui {
    private:
        Game& game; ///< Reference to the Coup game logic.
    
        sf::Font font; ///< The main font used for text rendering.
        std::string currentName; ///< Name currently typed by the user to add a player.
        std::vector<Player*> guiPlayers; ///< List of players for GUI rendering.
        std::vector<std::string> activeNames; ///< Active player names in the game.
    
        // Graphical components
        sf::RenderWindow window; ///< The main SFML window.
        sf::Text inputText; ///< Text field for entering new player names.
        sf::Text instruction; ///< Instruction label shown in the GUI.
    
        sf::RectangleShape startButton; ///< Button to start the game.
        sf::Text startText; ///< Label for the start button.
    
        sf::Texture menuBackgroundTexture; ///< Background texture for the menu screen.
        sf::Sprite menuBackgroundSprite; ///< Sprite used to render the menu background.
    
        sf::Texture gameBackgroundTexture; ///< Background texture for the game screen.
        sf::Sprite gameBackgroundSprite; ///< Sprite used to render the game background.
    
        std::vector<sf::RectangleShape> actionButtons; ///< GUI buttons for normal actions.
        std::vector<sf::Text> actionLabels; ///< Text labels for normal actions.
        std::vector<ActionType> actionTypes; ///< Corresponding action types.
    
        std::vector<sf::RectangleShape> targetButtons; ///< Buttons for choosing a target.
        std::vector<sf::Text> targetLabels; ///< Labels of target buttons.
    
        std::vector<SpecialActionButton> specialActionButtons; ///< Special action buttons for each player.
    
        bool awaitingTargetSelection = false; ///< Whether we're waiting for the user to select a target.
        ActionType selectedAction = ActionType::None; ///< Current action selected.
    
        std::string actionErrorMessage; ///< Error messages displayed to the user.
        std::string infoMessage; ///< Informational messages to display.
    
        SpecialActionInfo selectedSpecialAction; ///< If a special action requiring a target is selected.
        Player* specialActionOwner = nullptr; ///< Owner of the selected special action.
    
        sf::RectangleShape restartButton; ///< Button to reset the game.
        sf::Text restartText; ///< Label for the restart button.
    
        sf::RectangleShape quitButton; ///< Button to exit the game.
        sf::Text quitText; ///< Label for the quit button.
    
        /**
         * @brief Initializes fonts, buttons, and screen components.
         */
        void setupGUI();
    
        /**
         * @brief Handles user input and events such as button clicks or keyboard input.
         */
        void handleEvents();
    
        /**
         * @brief Executes a non-targeted action that was selected by the player.
         */
        void executeSelectedAction();
    
        /**
         * @brief Executes a targeted action on a specific player.
         * 
         * @param target The player being targeted by the current action.
         */
        void executeSelectedAction(Player& target);
    
        /**
         * @brief Main rendering loop, calls screen-specific render methods.
         */
        void render();
    
        /**
         * @brief Draws the in-game screen (players, actions, etc.).
         */
        void renderGameScreen();
    
        /**
         * @brief Draws the start screen for entering names and starting the game.
         */
        void renderStartScreen();
    
        /**
         * @brief Draws the end screen (winner display, restart option).
         */
        void renderEndScreen();
    
        /**
         * @brief Displays current player states (name, coins, roles, etc.).
         */
        void displayGameState();
    
        /**
         * @brief Displays the list of available actions to the current player.
         */
        void displayActionButtons();
    
        /**
         * @brief Displays the list of players that can be selected as targets.
         */
        void displayTargetSelection();
    
        /**
         * @brief Clears all target buttons from the screen.
         */
        void clearTargetSelection();
    
        /**
         * @brief Resets the game GUI state to the initial start screen.
         */
        void resetToStartState();
    
    public:
        /**
         * @brief Constructs a new GameGui instance tied to a game object.
         * 
         * @param game The game instance to be managed and visualized by this GUI.
         */
        GameGui(Game& game);
    
        /**
         * @brief Destroys the GameGui and closes the window if still open.
         */
        ~GameGui();
    
        /**
         * @brief Launches the main GUI event loop.
         */
        void run();
    };
}    