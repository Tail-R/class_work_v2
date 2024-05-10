#pragma once

#include "../shared.hpp"
#include "stage.hpp"
#include "../sprites/player.hpp"
#include "../sprites/enemy.hpp"
#include "../sprites/enemies.hpp"
#include "../socket/socket.hpp"

class Player;

class Stage1 : Stage {
public:
    Stage1(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);
    ~Stage1();

    StageTag start() override;

private:
    void spawn_worker() override;

    std::unique_ptr<std::thread> t_{nullptr};

    std::unique_ptr<Player> player_;
    std::shared_ptr<Enemies> enemies_;
};
