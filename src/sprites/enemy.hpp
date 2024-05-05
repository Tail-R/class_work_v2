#pragma once

#include "../shared.hpp"
#include "standalone.hpp"
#include "bullet.hpp"
#include "../factory/texture_factory.hpp"

class Enemy : Standalone {
public:
    Enemy(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);

    void set_bullet_dx(float dx);
    void set_bullet_dy(float dy);
    void set_bullet_health(int bullet_health);

    void update() override;

private:
    void fire();
    void update_bullets();

    std::vector<std::shared_ptr<Bullet>> bullets_;
    float bullet_dx_{0.0};
    float bullet_dy_{0.0};
    int bullet_health_{0};
};
