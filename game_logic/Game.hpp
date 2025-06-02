//noa.honigstein@gmail.com
#pragma once
#include <vector>
#include <deque>
#include <string>
#include "ActionType.hpp"
#include "DeletableActionType.hpp"
#include "../player/Player.hpp"
#include "../player/PlayerFactory.hpp"
#include "../player/SpecialActionUtils.hpp"


namespace coup {

    /**
     * @brief Represents a single game action performed by a player.
     * 
     * This structure records key information about an action taken during a game,
     * including who performed it, what type it was, the game turn, and an optional target.
     */
    struct Action {
        std::string playerName;         ///< The name of the player who performed the action.
        ActionType action;              ///< The type of action performed.
        DeletableActionType type;       ///< Type of action that may be reversed or deleted.
        size_t turn;                    ///< The round in which the action occurred.
        std::string target = "";        ///< Optional: name of the target player (if any).
    };

    /**
     * @brief Manages the entire game state and flow for the Coup game.
     * 
     * This class handles the list of players, turn management, action history,
     * and logic for starting, ending, and restarting games. It acts as the central
     * orchestrator of gameplay.
     */
    class Game {
    private:
        std::vector<Player*> playersList;           ///< List of players in the game.
        size_t currentTurnIndex = 0;            ///< Index of the player whose turn it is.
        size_t roundCounter = 0;                ///< Number of full rounds completed.
        bool started = false;                   ///< Whether the game has been started.
        std::string lastArrested = "";          ///< Last arrested player's name (prevents repeated arrests).
        std::deque<Action> actionHistory;       ///< Log of actions performed during the game.
        std::vector<std::string> activePlayersList; ///< Cached list of currently active player names.
        int playersAddedCount = 0;              ///< Number of players added since the last restart.

    public:
        Game() {};
        ~Game();

        Game(const Game& other) = delete;
        Game& operator=(const Game& other) = delete;
        Game(Game&& other) = delete;
        Game& operator=(Game&& other) = delete;

        /**
         * @brief Starts the game if enough players have been added.
         * @throws std::runtime_error if fewer than two players are present.
         */
        void start();

        /**
         * @brief Checks if the game has started.
         * @return true if the game has started.
         */
        bool isStarted() const;

        /**
         * @brief Checks whether the game is over.
         * @return true if one or fewer players are still active.
         */
        bool isOver() const;

        /**
         * @brief Restarts the game completely by clearing all players and state.
         */
        void restart();

        /**
         * @brief Returns the number of players that were added before the game started.
         * @return Number of initially added players.
         */
        int getStartPlayerCount() const;

        /**
         * @brief Creates a player with a random role and adds them to the game.
         * @param name The player's name.
         * @throws std::runtime_error if the maximum number of players (6) is reached.
         */
        void createPlayerRandomRole(const std::string& name);

        /**
         * @brief Finds a player by name.
         * @param name The name of the player to find.
         * @return Reference to the found player.
         * @throws std::runtime_error if the player doesn't exist.
         */
        Player& getPlayerByName(const std::string& name);

        /**
         * @brief Retrieves the list of all player pointers.
         * @return Vector of player pointers.
         */
        const std::vector<Player*>& getPlayers() const;

        const std::string& getLastArrested() const;
        void setLastArrested(const std::string& name);

        /**
         * @brief Adds a new player to the game.
         * @param player Pointer to the player to add.
         * @throws std::runtime_error if the max number of players is reached.
         * @throws std::invalid_argument if the name already exists.
         */
        void addPlayer(Player* player);

        /**
         * @brief Returns the current player whose turn it is.
         * @return Reference to the current player.
         */
        Player& currentPlayer() const;

        /**
         * @brief Returns the index of the current player.
         * @return Index in the players vector.
         */
        size_t getCurrentPlayerIndex() const;

        /**
         * @brief Checks if it's the specified player's turn.
         * @param player Pointer to the player to check.
         * @return true if it's that player's turn.
         */
        bool isPlayerTurn(const Player* player) const;

        /**
         * @brief Advances to the next player's turn.
         */
        void nextTurn();

        /**
         * @brief Ends the current player's turn, unless they have an extra action.
         */
        void endTurn();

        const int getActionHistorySize() const;
        const std::deque<Action>& getActionHistory() const;

        /**
         * @brief Logs an action performed by a player.
         * @param playerName Name of the acting player.
         * @param action Action type.
         * @param type Deletable action type.
         * @param targetName Optional target player's name.
         */
        void logAction(const std::string& playerName, ActionType action, DeletableActionType type, const std::string& targetName = "");

        bool hasRecentDeletableAction(const std::string& playerName, DeletableActionType type) const;
        void removeRecentDeletableAction(const std::string& playerName, DeletableActionType type);
        void erasePlayerAction(const std::string& playerName);

        /**
         * @brief Checks if an action requires a target player.
         * @param action The action to check.
         * @return true if a target is needed.
         */
        bool requiresTarget(ActionType action);

        /**
         * @brief Checks whether the player was the target of a recent Coup.
         * @param name Name of the player.
         * @return true if they were Couped recently.
         */
        bool wasCoupedRecently(const std::string& name) const;

        /**
         * @brief Returns the number of completed rounds.
         * @return Round counter value.
         */
        size_t getRoundCounter() const;

        /**
         * @brief Gets the list of currently active (alive) player names.
         * @return Vector of player names.
         */
        std::vector<std::string> activePlayers() const;

        /**
         * @brief Gets the name of the winner if the game is over.
         * @return Name of the winning player.
         * @throws std::runtime_error if the game is not over yet.
         */
        std::string winner() const;

        /**
         * @brief Eliminates a player from the game.
         * @param target The player to eliminate.
         */
        void eliminate(Player& target);

        /**
         * @brief Revives an eliminated player.
         * @param victim The player to bring back.
         */
        void revive(Player& victim);

        /**
         * @brief Gets the name of current player turn.
         * @return Name of the current player.
         * @throws std::runtime_error there no player or game not start yet.
         */
        std::string turn() const;

        /**
         * @brief Gets list of active players in game.
         * @return Name of the current player.
         */
        std::vector<std::string> players() const;


    };

}
