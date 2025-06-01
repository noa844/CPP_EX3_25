#include "GameGui.hpp"

#include "../player/PlayerFactory.hpp" 
#include "../lib/magic_enum.hpp"
#include <iostream>

using namespace sf;
using namespace std;

namespace coup {

    GameGui::GameGui(Game& game) : game(game), window(VideoMode(900, 800), "Coup - Game") {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            cerr << "Cannot load font!" << endl;
        }
                // === load menu background ===
        if (!menuBackgroundTexture.loadFromFile("assets/menu.png")) {
            std::cerr << "Failed to load menu background image!" << std::endl;
        }
        menuBackgroundSprite.setTexture(menuBackgroundTexture);
        menuBackgroundSprite.setScale(
            900.f / menuBackgroundTexture.getSize().x,
            800.f / menuBackgroundTexture.getSize().y
        );
      
        // === load game background ===
        if (!gameBackgroundTexture.loadFromFile("assets/game.png")) {
            std::cerr << "Failed to load game background image!" << std::endl;
        }
        gameBackgroundSprite.setTexture(gameBackgroundTexture);
        gameBackgroundSprite.setScale(
            900.f / gameBackgroundTexture.getSize().x,
            800.f / gameBackgroundTexture.getSize().y
        );
       
        setupGUI();
    }

    void GameGui::setupGUI() {
        float yInput = 155;
        float xInput= 125;
        //input
        inputText.setFont(font);
        inputText.setCharacterSize(20);
        inputText.setPosition(xInput, yInput);
        inputText.setFillColor(Color::White);
        //input message
        instruction.setFont(font);
        instruction.setString("Enter name and press Enter. Press START to begin.");
        instruction.setCharacterSize(20);
        instruction.setPosition(xInput, yInput + 30);
        instruction.setFillColor(Color(128, 0, 0));

        float yStart = 157;
        float xStart = 680;
        startButton.setSize(Vector2f(100, 40));
        startButton.setPosition(xStart, yStart);
        startButton.setFillColor(Color(166, 123, 91));
        startText.setFont(font);
        startText.setString("START");
        startText.setCharacterSize(20);
        startText.setPosition(xStart + 15, yStart + 5);
        startText.setFillColor(Color::Black);
        displayActionButtons();

        //Restart  Bouton 
        restartButton.setSize(sf::Vector2f(150, 40));
        restartButton.setPosition(350, 495);
        restartButton.setFillColor(sf::Color(112, 66, 20));
        restartText.setFont(font);
        restartText.setCharacterSize(20);
        restartText.setString("NEW GAME");
        restartText.setPosition(365, 500);
        restartText.setFillColor(sf::Color::White);

        // Quit  Bouton
        float yQuit = 70;
        float xQuit = 730;
        quitButton.setSize(sf::Vector2f(80, 30));
        quitButton.setPosition(xQuit, yQuit);
        quitButton.setFillColor(sf::Color(107, 47, 25));  
        quitText.setFont(font);
        quitText.setCharacterSize(16);
        quitText.setString("QUIT");
        quitText.setPosition(xQuit + 19, yQuit + 3);
        quitText.setFillColor(sf::Color::White);

    }

    void GameGui::displayActionButtons() {
        actionButtons.clear();
        actionLabels.clear();
        actionTypes.clear();
    
        float y = 180;
        float x = 700;
    
        for (auto action : magic_enum::enum_values<ActionType>()) {
            if (action == ActionType::None) continue; //ignore empty action
    
            // save 
            actionTypes.push_back(action);
    
            // create button
            sf::RectangleShape button(sf::Vector2f(120, 30));
            button.setPosition(x, y);
            button.setFillColor(sf::Color(128, 0, 0));
            actionButtons.push_back(button);
    
            // create label
            std::string label = std::string(magic_enum::enum_name(action));
            label[0] = std::toupper(label[0]);
    
            sf::Text text(label, font, 16);
            text.setPosition(x +10, y + 5);
            text.setFillColor(sf::Color::White);
            actionLabels.push_back(text);
    
            y += 50;
        }
    }
    
    
    void GameGui::handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            // CLOSE WINDOW
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //QUIT BUTTON
            if ( event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse = sf::Mouse::getPosition(window);
                if (quitButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    window.close();  
                    resetToStartState();

                }
            }
    
            // NAMES GETTER
            if (!game.isStarted()&& event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!currentName.empty()) currentName.pop_back();
                } else if (event.text.unicode == '\r') {
                    if (!currentName.empty()) {
                        try {
                            game.createPlayerRandomRole(currentName);
                            currentName.clear();
                            actionErrorMessage.clear();
                        } catch (const std::exception& e) {
                            actionErrorMessage = e.what();
                            std::cerr << "Error: " << e.what() << std::endl;
                        }
                    }
                } else if (event.text.unicode < 128) {
                    currentName += static_cast<char>(event.text.unicode);
                }
            }
    
            // Clic START bouton 
            if ( !game.isStarted() && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse = sf::Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    try {
                        game.start();
                        
                        guiPlayers = game.getPlayers();
                        actionErrorMessage.clear();
                    } catch (const std::exception& e) {
                        actionErrorMessage = e.what(); 
                        std::cerr << "Start error: " << e.what() << std::endl;
                    }
                }
            }            
    
            // CLIC ALL OTHER BUTTON
            if (game.isStarted() && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse = sf::Mouse::getPosition(window);
               
                if (restartButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    resetToStartState();
                }
                for (size_t i = 0; i < actionButtons.size(); ++i) {
                    if (actionButtons[i].getGlobalBounds().contains(mouse.x, mouse.y)) {
                        selectedAction = actionTypes[i];
                        actionErrorMessage.clear();
                        infoMessage.clear(); 

                        if (!specialActionOwner && game.requiresTarget(selectedAction)) {
                            displayTargetSelection();
                        } else {
                            executeSelectedAction();
                            clearTargetSelection();
                        }
                    }
                }

                // target selection
                if (game.requiresTarget(selectedAction)) {
                    for (size_t i = 0; i < targetButtons.size(); ++i) {
                        if (targetButtons[i].getGlobalBounds().contains(mouse.x, mouse.y)) {
                            std::string targetName = targetLabels[i].getString().toAnsiString();
                            actionErrorMessage.clear();
                            infoMessage.clear(); 
                            try {
                                Player& target = game.getPlayerByName(targetName);
                                executeSelectedAction(target);
                                clearTargetSelection();
                            } catch (const std::exception& e) {
                                actionErrorMessage = e.what();
                            }
                        }
                    }
                }

                // === clic specials button===
                for (auto& btn : specialActionButtons) {
                    if (btn.shape.getGlobalBounds().contains(mouse.x, mouse.y)) {
                        try {

                            Player& p = game.getPlayerByName(btn.playerName);

                            if (!p.isActive()) continue;

                            // searche special action info
                            std::vector<SpecialActionInfo> specials = p.getSpecialActions();
                            auto it = std::find_if(specials.begin(), specials.end(), [&](const SpecialActionInfo& info) {
                                return info.name == btn.actionType;
                                
                            });

                            if (it == specials.end()) {
                                actionErrorMessage = "Action not found.";
                                continue;
                            }

                            const SpecialActionInfo& info = *it;

                            // if not your turn and the action can't be call anytime.
                            if (!game.isPlayerTurn(&p) && !info.canBeUsedAnytime) {
                                actionErrorMessage = "You cannot use this special action right now.";
                                continue;
                            }

                            //if action requires target
                            if (info.requiresTarget) {
                                selectedSpecialAction = info; 
                                specialActionOwner = &p;
                                selectedAction = ActionType::None; 
                                displayTargetSelection();     
                                actionErrorMessage = "Select a target for special action.";
                            } else {
                                p.executeSpecialAction(info.name);
                                actionErrorMessage.clear();
                            }

                        } catch (const std::exception& e) {
                            actionErrorMessage = e.what();
                        }
                    }
                }

                // === Clic target for special action ===
                if (specialActionOwner && selectedSpecialAction.requiresTarget) {
                    for (size_t i = 0; i < targetButtons.size(); ++i) {
                        if (targetButtons[i].getGlobalBounds().contains(mouse.x, mouse.y)) {
                            std::string targetName = targetLabels[i].getString().toAnsiString();
                            try {
                                Player& target = game.getPlayerByName(targetName);
                                            
                              
                                if (selectedSpecialAction.name == AllSpecialActionType::Peek_Coins) {
                                    specialActionOwner->executeSpecialAction(selectedSpecialAction.name, &target);
                                    int coins = target.getCoinsCount();
                                    infoMessage = target.getName() + " has " + std::to_string(coins) + " coins.";
                                } else {
                                    specialActionOwner->executeSpecialAction(selectedSpecialAction.name, &target);
                                    actionErrorMessage.clear();
                                    infoMessage.clear(); 
                                }
                                clearTargetSelection();
                                specialActionOwner = nullptr;
                                
        
                            } catch (const std::exception& e) {
                                actionErrorMessage = e.what();
                            }
                            
                        }
                    }
                    
                }

                

            }
        }
    }


    void GameGui::clearTargetSelection() {
        targetButtons.clear();
        targetLabels.clear();
    }

    void GameGui::displayGameState() {
        float y = 170;
        const float xText = 75;      // player name position 
    
        specialActionButtons.clear();  
    
        for (size_t i = 0; i < guiPlayers.size(); ++i) {
            Player* p = guiPlayers[i];
    
            // === display player infos  ===
            sf::Text txt;
            txt.setFont(font);
            txt.setCharacterSize(20);
            txt.setPosition(xText, y);
    
            if (!p->isActive()) {
                txt.setFillColor(Color(128, 128, 128)); 
            } else if (i == game.getCurrentPlayerIndex()) {
                txt.setFillColor(Color(193, 154, 107)); 
            } else {
                txt.setFillColor(Color::White);
            }
    
            std::string status = p->isActive() ? "Active" : "Eliminated";
            txt.setString(
                p->getName() + " | Role: " + p->getRoleName() +
                " | Coins: " + std::to_string(p->getCoinsCount()) +
                " | " + status
            );
            window.draw(txt);
    
            // === display special buttons ===
            std::vector<SpecialActionInfo> actions = p->getSpecialActions();
            float btnY = y + 30; 
            for (size_t j = 0; j < actions.size(); ++j) {
                SpecialActionButton btn;
                btn.playerName = p->getName();
                if(!p->isActive()){continue;}
                btn.actionType = actions[j].name;
    
                btn.shape.setSize(sf::Vector2f(100, 25));
                btn.shape.setPosition(xText, btnY);
                btn.shape.setFillColor(sf::Color(255, 230, 200));
    
                btn.label.setFont(font);
                btn.label.setString(std::string(magic_enum::enum_name(actions[j].name)));
                btn.label.setCharacterSize(14);
                btn.label.setPosition(xText + 5, btnY + 2);
                btn.label.setFillColor(sf::Color::Black);
    
                specialActionButtons.push_back(btn);

                btnY += 30;
            }
    
            y = btnY + 10;
        }
    }
    

    void GameGui::executeSelectedAction() {
        try {
            Player& current = game.currentPlayer();
            switch (selectedAction) {
                case ActionType::Tax:
                    current.tax();
                    break;
                case ActionType::Gather:
                    current.gather();
                    break;
                case ActionType::Bribe:
                    current.bribe();
                    break;
                default:
                    return;
            }
            clearTargetSelection();

        } catch (const std::exception& e) {
            actionErrorMessage = e.what();
        }
    }
    
    void GameGui::executeSelectedAction(Player& target) {
        try {
            Player& current = game.currentPlayer();
            switch (selectedAction) {
                case ActionType::Arrest:
                    current.arrest(target);
                    break;
                case ActionType::Sanction:
                    current.sanction(target);
                    break;
                case ActionType::Coup:
                    current.coup(target);
                    break;
                default:
                    return;
            }
            clearTargetSelection();

        } catch (const std::exception& e) {
            actionErrorMessage = e.what();
        }
    }
    

    void GameGui::displayTargetSelection() {
        targetButtons.clear();
        targetLabels.clear();
    
        float y = 200;
        const float x = 500;
        std::string currentName = game.currentPlayer().getName();
    
        activeNames = game.activePlayers();
    
        for (const auto name : guiPlayers) {
    
            sf::RectangleShape btn(sf::Vector2f(120, 30));
            btn.setPosition(x, y);
            btn.setFillColor(sf::Color(210, 180, 140));
            targetButtons.push_back(btn);
    
            sf::Text label(name->getName(), font, 16);
            label.setPosition(x+10, y + 5);
            label.setFillColor(sf::Color::White);
            targetLabels.push_back(label);
    
            y += 40;
        }
    }
    
    void GameGui::render() {
        window.clear();
    
        if (game.isOver()) {
            window.draw(gameBackgroundSprite);
            renderEndScreen();
        } else if (!game.isStarted()) {
            window.draw(menuBackgroundSprite);
            renderStartScreen();
        } else {
            window.draw(gameBackgroundSprite);
            renderGameScreen();
        }
    
        
        if (!actionErrorMessage.empty()) {
            sf::Text errText(actionErrorMessage, font, 18);
            errText.setFillColor(sf::Color::Red);
            errText.setPosition(75, 700);
            window.draw(errText);
        }
        if (!infoMessage.empty()) {
            sf::Text infoText(infoMessage, font, 18);
            infoText.setFillColor(sf::Color(80, 200, 120)); 
            infoText.setPosition(75, 600);
            window.draw(infoText);
        }

        window.draw(quitButton);
        window.draw(quitText);
    
        window.display();
    }
    
    void GameGui::renderStartScreen() {
        inputText.setString("Name: " + currentName);
        window.draw(inputText);
        window.draw(instruction);
        window.draw(startButton);
        window.draw(startText);
        
        
    }

    void GameGui::renderGameScreen() {
        // current player
        sf::Text turnText("Current turn: " + game.currentPlayer().getName(), font, 22);
        turnText.setPosition(75, 120);
        turnText.setFillColor(sf::Color(128, 0, 0));
        window.draw(turnText);
        
    
        
        displayGameState();
    
        // action buttons
        for (size_t i = 0; i < actionButtons.size(); ++i) {
            window.draw(actionButtons[i]);
            window.draw(actionLabels[i]);
        }
    
        // target buttons
        for (size_t i = 0; i < targetButtons.size(); ++i) {
            window.draw(targetButtons[i]);
            window.draw(targetLabels[i]);
        }
    
        // special action buttons
        for (const auto& btn : specialActionButtons) {
            window.draw(btn.shape);
            window.draw(btn.label);
        }
    
        
    }

    void GameGui::renderEndScreen() {
        sf::Text winText;
        winText.setFont(font);
        winText.setCharacterSize(30);
        winText.setFillColor(sf::Color(128, 0, 0));
        winText.setString("Winner: " + game.winner());
        winText.setPosition(300, 250);
        window.draw(winText);
       
    
        // Restart button
        window.draw(restartButton);
        window.draw(restartText);
    
    }
    
    
    void GameGui::resetToStartState() {
        game.restart();  // on vide les données du jeu
        guiPlayers.clear();
        targetButtons.clear();
        targetLabels.clear();
        specialActionButtons.clear();
        currentName.clear();
        selectedAction = ActionType::None;
        selectedSpecialAction = {};
        specialActionOwner = nullptr;
        infoMessage.clear();
        actionErrorMessage.clear();
    }
    
    
    GameGui::~GameGui() {
    
        guiPlayers.clear();
        actionButtons.clear();
        actionLabels.clear();
        targetButtons.clear();
        targetLabels.clear();


    }
    

    void GameGui::run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

}
