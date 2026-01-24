#pragma once
enum class GameState {
    MENU,
    PLAYING,
    QUIT,
    Idle,
    MovingLeft,
    MovingRight
};

enum class PlayerDirection {
    None,
    Left,
    Right,
    Up,
};