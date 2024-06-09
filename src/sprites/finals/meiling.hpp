#pragma once

#include "../../shared.hpp"
#include "../enemy.hpp"
#include "../../factory/texture_factory.hpp"

class Meiling final : public Enemy {
public:
    Meiling(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);
    void update() override;

private:
    int frame_count_{0};

    void spiral_shot(int f);
    void random_shot();
};