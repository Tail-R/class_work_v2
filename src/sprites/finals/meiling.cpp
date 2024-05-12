#include "meiling.hpp"

Meiling::Meiling(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : Enemy{ctx, factory} {}

void Meiling::spiral_shot(int f) {
    if (f % 13 == 0) {
        float two_pi = 2 * M_PI;
        float piece = two_pi / 10;

        for (float i = f; i < two_pi + f; i += piece) {
            set_bullet_dx(cos(i) * 2);
            set_bullet_dy(sin(i) * 2);
            fire();
        }
    }
}

void Meiling::random_shot() {
    float two_pi = 2 * M_PI;
    float piece = two_pi / 30;

    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::uniform_real_distribution<float> dist(-6.0f, 6.0f);

    for (float i = 0; i < two_pi; i += piece) {
        set_bullet_dx(cos(i) * dist(engine));
        set_bullet_dy(sin(i) * dist(engine));
        fire();
    }
}

void Meiling::update() {
    if (frame_count_ == 0)
    {
        set_dx(-4); set_dy(2);
    }
    else if (frame_count_ == 75)
    {
        set_dx(0); set_dy(0);
    }
    else if (frame_count_ >= 160 && 320 >= frame_count_)
    {
        spiral_shot(frame_count_);
    }
    else if (frame_count_ == 360)
    {
        random_shot();
        set_dx(2); set_dy(-1);
    }
    else if (frame_count_ == 435) {
        set_dx(0); set_dy(0);
    }
    else if (frame_count_ >= 440 && 600 >= frame_count_)
    {
        spiral_shot(frame_count_);
    }
    else if (frame_count_ == 620)
    {
        random_shot();
        set_dx(-2); set_dy(1);
    }
    else if (frame_count_ == 695)
    {
        set_dx(0); set_dy(0);
    }
    else if (frame_count_ >= 720 && 880 >= frame_count_)
    {
        spiral_shot(frame_count_);
    }
    else if (frame_count_ > 880)
    {
        random_shot();
        frame_count_ = 75;
    }

    move();
    update_bullets();
    render();

    frame_count_++;
}
