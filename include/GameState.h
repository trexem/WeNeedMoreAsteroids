#pragma once

#include <iostream>
#include <unordered_map>

enum class GameState {
    MainMenu,
    Playing,
    LevelUp,
    Paused,
    Settings,
    GameOver,
    Restart,
    UpgradeStore,
    Quit
};

inline const std::unordered_map<GameState, std::string> GameStateNames = {
    {GameState::MainMenu,       "MainMenu"},
    {GameState::Playing,        "Playing"},
    {GameState::LevelUp,        "LevelUp"},
    {GameState::Paused,         "Paused"},
    {GameState::Settings,       "Settings"},
    {GameState::GameOver,       "GameOver"},
    {GameState::UpgradeStore,   "UpgradeStore"},
    {GameState::Restart,        "Restart"},
    {GameState::Quit,           "Quit"}
};

inline std::string getGameStateName(GameState type) {
    if (auto it = GameStateNames.find(type); it != GameStateNames.end()) {
        return it->second;
    }
    return "Unknown";
}

inline std::ostream& operator<<(std::ostream& os, GameState state) {
    os << getGameStateName(state); // or getGameStatePosition(type)
    return os;
}
