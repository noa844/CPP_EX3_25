
#pragma once
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "../game_logic/Game.hpp"
#include "../player/Player.hpp"

namespace coup {

class GameGUI : public QMainWindow {
    Q_OBJECT
public:
    GameGUI(Game* game, QWidget* parent = nullptr);

private slots:
    void onGather();
    void onTax();
    void onBribe();
    void onArrest();
    void onSanction();
    void onCoup();
    void updateUI();

private:
    coup::Game* game;
    QListWidget* playerList;
    QLabel* currentLabel;
    QPushButton* btnGather;
    QPushButton* btnTax;
    QPushButton* btnBribe;
    QPushButton* btnArrest;
    QPushButton* btnSanction;
    QPushButton* btnCoup;
};
}