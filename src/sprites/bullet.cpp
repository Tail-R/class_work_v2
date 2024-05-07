#include "bullet.hpp"

Bullet::Bullet(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : Standalone{ctx, factory} {}

void Bullet::set_health(int health) { health_ = health > 0 ? health : 0; }

int Bullet::get_health() { return health_; }

void Bullet::update() {
    move();
    load_texture();
    render();
}
