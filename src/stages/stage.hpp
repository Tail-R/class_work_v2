#pragma once

#include "../shared.hpp"
#include "../factory/texture_factory.hpp"

typedef enum {
    SPLASH,
    STAGE_1,
    EXIT
} StageTag;

class Stage {
public:
    Stage(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);

    virtual StageTag start();

    SDL_Renderer* get_renderer();
    std::shared_ptr<TextureFactory> get_factory();

private:
    virtual void spawn_worker();

    SDL_Renderer* ctx_;
    std::shared_ptr<TextureFactory> factory_;
};
