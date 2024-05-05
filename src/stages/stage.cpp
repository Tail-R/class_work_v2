#include "stage.hpp"

Stage::Stage(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : ctx_{ctx}
    , factory_{factory} {}

StageTag Stage::start() {
    return EXIT;
}

SDL_Renderer* Stage::get_renderer() { return ctx_; }
std::shared_ptr<TextureFactory> Stage::get_factory() { return factory_; }

void Stage::spawn_worker() { return; }
