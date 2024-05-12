#include "standalone.hpp"

Standalone::Standalone(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : Sprite{ctx, factory} {}

void Standalone::set_dx(float dx) { dx_ = dx; }
void Standalone::set_dy(float dy) { dy_ = dy; }
void Standalone::set_health(int health) { health_ = health > 0 ? health : 0; }

void Standalone::update() {
    move();
    render();
}

void Standalone::move() {
    auto region = get_region();

    auto next_x = region.x + dx_;
    auto next_y = region.y + dy_;

    bool expr1 = (next_x >= 0) && (WINDOW_WIDTH + region.w >= next_x);
    bool expr2 = (next_y >= 0) && (WINDOW_HEIGHT + region.h >= next_y);

    set_x(next_x);
    set_y(next_y);

    dead_= !(expr1 && expr2);
}

bool Standalone::collided_with(std::shared_ptr<Sprite> sprite) {
    auto s1 = get_region();
    auto s2 = sprite->get_region();

    bool expr1 = (s1.x <= s2.x + s2.w) && (s2.x <= s1.x + s1.w);
    bool expr2 = (s1.y <= s2.y + s2.h) && (s2.y <= s1.y + s1.h);

    return expr1 && expr2;
}

bool Standalone::is_dead() { return dead_; }
int Standalone::get_health() { return health_; }
