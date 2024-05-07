#include "enemies.hpp"

void Enemies::lock() { mtx_.lock(); }
void Enemies::unlock() {mtx_.unlock(); }

void Enemies::update() {
    for (auto enemy : sprites_)
        enemy->update();
}

std::vector<std::shared_ptr<Enemy>>& Enemies::get_sprites() {
    return sprites_;
}
