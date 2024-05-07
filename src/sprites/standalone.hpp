#pragma once

#include "../shared.hpp"
#include "sprite.hpp"
#include "../factory/texture_factory.hpp"

class Standalone : public Sprite {
public:
    Standalone(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);

    void set_dx(float dx);
    void set_dy(float dy);

    void update() override;
    void move();
    bool collided_with(std::shared_ptr<Sprite> sprite);
    bool stickouted();

private:
    float dx_{0.0};
    float dy_{0.0};

    bool stickouted_{false};
};
