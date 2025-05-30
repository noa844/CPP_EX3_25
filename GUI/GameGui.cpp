#include "GameGui.hpp"
#include "../player/PlayerFactory.hpp" 
#include <iostream>

using namespace sf;
using namespace std;

namespace coup {

    GameGui::GameGui(Game& game) : game(game), window(VideoMode(800, 600), "Coup - Game") {
        if (!font.loadFromFile("arial.ttf")) {
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
    }

    void GameGui::handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (!gameStarted && event.type == Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!currentName.empty()) currentName.pop_back();
                } else if (event.text.unicode == '\r') {
                    if (!currentName.empty()) {
                        game.createPlayerRandomRole(currentName);
                        currentName.clear();
                    }
                } else if (event.text.unicode < 128) {
                    currentName += static_cast<char>(event.text.unicode);
                }
            }

            if (!gameStarted && event.type == Event::MouseButtonPressed) {
                Vector2i mouse = Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    if (game.getPlayers().size() >= 2) {
                        game.start();
                        gameStarted = true;
                        guiPlayers = game.getPlayers(); // snapshot
                    } else {
                        cout << "Minimum 2 players required!" << endl;
                    }
                }
            }

            if (gameStarted && event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                game.nextTurn();
                currentPlayerIndex = (currentPlayerIndex + 1) % game.getPlayers().size();
            }
        }
    }

    void GameGui::displayGameState() {
        float y = 150;
        for (size_t i = 0; i < guiPlayers.size(); ++i) {
            Text txt;
            txt.setFont(font);
            txt.setCharacterSize(20);
            txt.setFillColor(i == currentPlayerIndex ? Color::Red : Color::White);
            txt.setPosition(20, y);
            txt.setString(guiPlayers[i]->getName() + " | Role: " + guiPlayers[i]->getRoleName() + " | Coins: " + to_string(guiPlayers[i]->getCoinsCount()));
            window.draw(txt);
            y += 30;
        }
    }

    void GameGui::render() {
        window.clear();

        if (!gameStarted) {
            inputText.setString("Name: " + currentName);
            window.draw(inputText);
            window.draw(instruction);
            window.draw(startButton);
            window.draw(startText);
        } else {
            Text turnText("Current turn: " + game.currentPlayer().getName(), font, 24);
            turnText.setPosition(20, 100);
            turnText.setFillColor(Color::Cyan);
            window.draw(turnText);

            displayGameState();
        }

        window.display();
    }

    void GameGui::run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

}
