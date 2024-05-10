#pragma once

#include "../shared.hpp"
#include "sprite.hpp"
#include "standalone.hpp"
#include "../factory/texture_factory.hpp"

class Bullet : public Standalone {
public:
    Bullet(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);

    void update() override;

private:
};
