#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <boost/json.hpp>

#include <sys/socket.h>
#include <sys/un.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>
#include <thread>
#include <mutex>
#include <atomic>
#include <cmath>
#include <random>
#include <condition_variable>
#include <optional>
#include <format>
#include <chrono>

// Window properties
constexpr std::string_view WINDOW_TITLE = "Bullet Hell";
constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 800;
constexpr int WINDOW_FPS = 60;
constexpr int WINDOW_FRAME_LENGTH = 1000 / WINDOW_FPS;

// Renderer properties
constexpr SDL_RendererFlags RENDERER_FLAGS = SDL_RENDERER_ACCELERATED;
constexpr Uint32 IMG_FLAGS = IMG_INIT_PNG | IMG_INIT_JPG;

// Player properties
constexpr float PLAYER_WIDTH = 20.0;
constexpr float PLAYER_HEIGHT = 20.0;
constexpr float PLAYER_BASE_SPEED = 2.0;
constexpr float PLAYER_BULLET_WIDTH = 10.0;
constexpr float PLAYER_BULLET_HEIGHT = 10.0;
constexpr float PLAYER_BULLET_DX = 0.0;
constexpr float PLAYER_BULLET_DY = -10.0;
constexpr int PLAYER_BULLET_RATE = 20;
constexpr int PLAYER_BULLET_HEALTH = 1;
constexpr std::string_view PLAYER_TEXTURE = "images/player_bullet.png";
constexpr std::string_view PLAYER_BULLET_TEXTURE = "images/player_bullet.png";

// Enemy properties
constexpr float ENEMY_WIDTH = 20.0;
constexpr float ENEMY_HEIGHT = 20.0;
constexpr float ENEMY_BULLET_WIDTH = 10.0;
constexpr float ENEMY_BULLET_HEIGHT = 10.0;
constexpr int ENEMY_BULLET_HEALTH = 1;
constexpr std::string_view ENEMY_TEXTURE = "images/enemy_bullet.png";
constexpr std::string_view ENEMY_BULLET_TEXTURE = "images/enemy_bullet.png";

// Misc
constexpr std::string_view TIME_ZONE = "Asia/Tokyo";
constexpr const char* SOCKET_NAME = "/tmp/bullet_hell.sock";
constexpr int SOCKET_BUFF_SIZE = 16384;

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
