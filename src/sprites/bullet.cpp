#include "bullet.hpp"

Bullet::Bullet(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : Standalone{ctx, factory} {}

void Bullet::update() {
    move();
    update_texture();
    render();
}
