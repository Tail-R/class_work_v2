#pragma once

#include "../shared.hpp"
#include "sprite.hpp"
#include "../factory/texture_factory.hpp"

class Standalone : public Sprite {
public:
    Standalone(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);

    void set_dx(float dx);
    void set_dy(float dy);
    void set_health(int health);

    void update() override;
    void move();

    bool collided_with(std::shared_ptr<Sprite> sprite);
    bool is_dead();
    int get_health();

private:
    float dx_{0.0};
    float dy_{0.0};
    int health_{0};

    bool dead_{false};
};
