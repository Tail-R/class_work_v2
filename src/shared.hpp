#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <optional>
#include <format>
#include <chrono>

constexpr std::string_view TIME_ZONE = "Asia/Tokyo";

// Window properties
constexpr std::string_view WINDOW_TITLE = "Bullet Hell";
constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 800;
constexpr int WINDOW_FPS = 60;
constexpr int WINDOW_FRAME_LENGTH = 1000 / WINDOW_FPS;

// Player properties
constexpr float PLAYER_WIDTH = 20;
constexpr float PLAYER_HEIGHT = 20;
constexpr float PLAYER_BULLET_WIDTH = 10;
constexpr float PLAYER_BULLET_HEIGHT = 10;
constexpr std::string_view PLAYER_TEXTURE = "images/player_bullet.png";
constexpr std::string_view PLAYER_BULLET_TEXTURE = "images/player_bullet.png";

// Enemy properties
constexpr float ENEMY_WIDTH = PLAYER_WIDTH;
constexpr float ENEMY_HEIGHT = PLAYER_HEIGHT;
constexpr float ENEMY_BULLET_WIDTH = PLAYER_BULLET_WIDTH;
constexpr float ENEMY_BULLET_HEIGHT = PLAYER_BULLET_HEIGHT;
constexpr std::string_view ENEMY_TEXTURE = "images/enemy_bullet.png";
constexpr std::string_view ENEMY_BULLET_TEXTURE = "images/enemy_bullet.png";

#define DEBUG

template <typename... Args>
void debug_log(std::format_string<Args...> fmt, Args&&... args) {
#ifdef DEBUG
    using namespace std;

    auto now = chrono::floor<chrono::milliseconds>
        (chrono::system_clock::now());

    cout << format("{:%H:%m:%S} {}",
        chrono::zoned_time{TIME_ZONE, now},
        format(fmt, forward<Args>(args)...)
    ) << endl;
#else
    return;
#endif
}
