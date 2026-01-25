#pragma once
enum class GameState {
    MENU,
    PLAYING,
    QUIT,
    Idle,
    movingLeft,
    movingRight
};

enum class PlayerDirection {
    None,
    Left,
    Right,
    Up,
};