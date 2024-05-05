#pragma once

#include "../shared.hpp"
#include "stage.hpp"
#include "../sprites/player.hpp"
#include "../sprites/enemy.hpp"

class Stage1 : Stage {
public:
    Stage1(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);

    StageTag start() override;

private:
    void spawn_worker() override;
    // std::mutex mtx_;

    std::unique_ptr<Player> player_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
};
