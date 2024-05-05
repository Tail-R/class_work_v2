#pragma once

#include "../shared.hpp"
#include "sprite.hpp"
#include "standalone.hpp"
#include "../factory/texture_factory.hpp"

class Bullet : public Standalone {
public:
    Bullet(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);

    void set_health(int health);
    int get_health();

private:
    int health_{0};
};
