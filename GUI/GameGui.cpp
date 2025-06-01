#include "GameGui.hpp"
#include "../player/PlayerFactory.hpp" 
#include "../lib/magic_enum.hpp"
#include <iostream>

using namespace sf;
using namespace std;

namespace coup {

    GameGui::GameGui(Game& game) : game(game), window(VideoMode(800, 700), "Coup - Game") {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            cerr << "Cannot load font!" << endl;
        }
        setupGUI();
    }

    void GameGui::setupGUI() {
        inputText.setFont(font);
        inputText.setCharacterSize(20);
        inputText.setPosition(20, 20);
        inputText.setFillColor(Color::White);

        instruction.setFont(font);
        instruction.setString("Enter name and press Enter. Press START to begin.");
        instruction.setCharacterSize(20);
        instruction.setPosition(20, 50);
        instruction.setFillColor(Color::Yellow);

        startButton.setSize(Vector2f(100, 40));
        startButton.setPosition(650, 20);
        startButton.setFillColor(Color::Green);

        startText.setFont(font);
        startText.setString("START");
        startText.setCharacterSize(20);
        startText.setPosition(660, 25);
        startText.setFillColor(Color::Black);
        displayActionButtons();

    }

    void GameGui::displayActionButtons() {
        actionButtons.clear();
        actionLabels.clear();
        actionTypes.clear();
    
        float y = 180;
        float x = 650;
    
        for (auto action : magic_enum::enum_values<ActionType>()) {
            if (action == ActionType::None) continue; // on ignore l'action vide
    
            // Stocker l'action
            actionTypes.push_back(action);
    
            // Créer le bouton
            sf::RectangleShape button(sf::Vector2f(120, 30));
            button.setPosition(x, y);
            button.setFillColor(sf::Color(100, 100, 255));
            actionButtons.push_back(button);
    
            // Créer le label
            std::string label = std::string(magic_enum::enum_name(action));
            label[0] = std::toupper(label[0]); // Optionnel : mettre la première lettre en majuscule
    
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
    
            // CLIC ACTION'S BUTTON
            if (game.isStarted() && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < actionButtons.size(); ++i) {
                    if (actionButtons[i].getGlobalBounds().contains(mouse.x, mouse.y)) {
                        selectedAction = actionTypes[i];
                        actionErrorMessage.clear();
                        infoMessage.clear(); 

                        if (game.requiresTarget(selectedAction)) {
                            displayTargetSelection();
                        } else {
                            executeSelectedAction();
                            clearTargetSelection();
                        }
                    }
                }

                // Sélection d'une cible
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

                // === Gestion du clic sur les boutons spéciaux ===
                for (auto& btn : specialActionButtons) {
                    if (btn.shape.getGlobalBounds().contains(mouse.x, mouse.y)) {
                        try {
                            Player& p = game.getPlayerByName(btn.playerName);

                            if (!p.isActive()) continue;

                            // Trouver les infos sur cette action spéciale
                            std::vector<SpecialActionInfo> specials = p.getSpecialActions();
                            auto it = std::find_if(specials.begin(), specials.end(), [&](const SpecialActionInfo& info) {
                                return info.name == btn.actionType;
                            });

                            if (it == specials.end()) {
                                actionErrorMessage = "Action not found.";
                                continue;
                            }

                            const SpecialActionInfo& info = *it;

                            // Si ce n’est pas son tour et l'action n’est pas 'anytime'
                            if (!game.isPlayerTurn(&p) && !info.canBeUsedAnytime) {
                                actionErrorMessage = "You cannot use this special action right now.";
                                continue;
                            }

                            // Si l'action nécessite une cible
                            if (info.requiresTarget) {
                                selectedSpecialAction = info; // Mémoriser pour plus tard
                                specialActionOwner = &p;
                                displayTargetSelection();     // Afficher les cibles
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

                // === Clic sur une cible pour une action spéciale ===
                if (specialActionOwner && selectedSpecialAction.requiresTarget) {
                    for (size_t i = 0; i < targetButtons.size(); ++i) {
                        if (targetButtons[i].getGlobalBounds().contains(mouse.x, mouse.y)) {
                            std::string targetName = targetLabels[i].getString().toAnsiString();
                            try {
                                Player& target = game.getPlayerByName(targetName);
                                specialActionOwner->executeSpecialAction(selectedSpecialAction.name, &target);
                                if (selectedSpecialAction.name == AllSpecialActionType::Peek_Coins) {
                                    int coins = target.getCoinsCount();
                                    infoMessage = target.getName() + " has " + std::to_string(coins) + " coins.";
                                } else {
                                    specialActionOwner->executeSpecialAction(selectedSpecialAction.name, &target);
                                    actionErrorMessage.clear();
                                    infoMessage.clear(); 
                                }
                                clearTargetSelection();
                                specialActionOwner = nullptr;
                                actionErrorMessage.clear();
        
                            } catch (const std::exception& e) {
                                actionErrorMessage = e.what();
                            }
                            return;
                        }
                    }
                    return;
                }

                

            }
        }
    }


    void GameGui::clearTargetSelection() {
        targetButtons.clear();
        targetLabels.clear();
    }

    void GameGui::displayGameState() {
        float y = 150;
        const float xText = 20;      // Position du nom du joueur
    
        specialActionButtons.clear();  // On vide la liste avant d'en ajouter
    
        for (size_t i = 0; i < guiPlayers.size(); ++i) {
            Player* p = guiPlayers[i];
    
            // === Affichage des infos du joueur ===
            sf::Text txt;
            txt.setFont(font);
            txt.setCharacterSize(20);
            txt.setPosition(xText, y);
    
            if (!p->isActive()) {
                txt.setFillColor(Color(128, 128, 128)); // gris si éliminé
            } else if (i == game.getCurrentPlayerIndex()) {
                txt.setFillColor(Color(180, 200, 255)); // rouge pour le joueur courant
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
    
            // === Affichage des boutons spéciaux ===
            std::vector<SpecialActionInfo> actions = p->getSpecialActions();
            float btnY = y + 30; 
            for (size_t j = 0; j < actions.size(); ++j) {
                SpecialActionButton btn;
                btn.playerName = p->getName();
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
    

    void GameGui::render() {
        window.clear();
    
        if (!game.isStarted()) {
            // === Phase de saisie des noms avant début du jeu ===
            inputText.setString("Name: " + currentName);
            window.draw(inputText);
            window.draw(instruction);
            window.draw(startButton);
            window.draw(startText);
        } else {
            // === Affichage du tour courant ===
            sf::Text turnText("Current turn: " + game.currentPlayer().getName(), font, 24);
            turnText.setPosition(20, 100);
            turnText.setFillColor(sf::Color::Cyan);
            window.draw(turnText);
    
            // === Affichage de tous les joueurs + leurs boutons spéciaux ===
            displayGameState();
    
            // === Affichage des boutons d'action classiques ===
            for (size_t i = 0; i < actionButtons.size(); ++i) {
                window.draw(actionButtons[i]);
                window.draw(actionLabels[i]);
            }
    
            // === Affichage des boutons de sélection de cibles ===
            for (size_t i = 0; i < targetButtons.size(); ++i) {
                window.draw(targetButtons[i]);
                window.draw(targetLabels[i]);
            }
    
            // === Affichage des boutons spéciaux ===
            for (const auto& btn : specialActionButtons) {
                window.draw(btn.shape);
                window.draw(btn.label);
            }
        }
    
        // === Affichage des messages d'erreur ===
        if (!actionErrorMessage.empty()) {
            sf::Text errText(actionErrorMessage, font, 18);
            errText.setFillColor(sf::Color::Red);
            errText.setPosition(20, 700);
            window.draw(errText);
        }

        if (!infoMessage.empty()) {
            sf::Text infoText(infoMessage, font, 18);
            infoText.setFillColor(sf::Color(80, 200, 120)); // vert doux
            infoText.setPosition(20, 750);
            window.draw(infoText);
        }
    
        window.display();
    }
    

    void GameGui::displayTargetSelection() {
        targetButtons.clear();
        targetLabels.clear();
    
        float y = 200;
        const float x = 450;
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
