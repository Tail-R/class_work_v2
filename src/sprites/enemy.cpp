#include "enemy.hpp"

Enemy::Enemy(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : Standalone{ctx, factory} {

    id_manager_ = std::make_unique<UniqueIDResolver>();
}

void Enemy::set_bullet_dx(float bullet_dx) { bullet_dx_ = bullet_dx; }
void Enemy::set_bullet_dy(float bullet_dy) { bullet_dy_ = bullet_dy; }
void Enemy::set_bullet_health(int bullet_health) { bullet_health_ = bullet_health; }

void Enemy::lock() { mtx_.lock(); }
void Enemy::unlock() {mtx_.unlock(); }

void Enemy::update() {
    move();
    update_texture();
    update_bullets();
    render();
}

void Enemy::fire() {
    auto ctx = get_renderer();
    auto factory = get_factory();
    auto region = get_region();
    auto bullet = std::make_shared<Bullet>(ctx, factory);
    auto new_id = id_manager_->generate_available_id();

    bullet->set_region(
        region.x,
        region.y,
        ENEMY_BULLET_WIDTH,
        ENEMY_BULLET_HEIGHT
    );

    bullet->set_dx(bullet_dx_);
    bullet->set_dy(bullet_dy_);
    bullet->set_health(bullet_health_);
    bullet->set_texture_path(ENEMY_BULLET_TEXTURE);
    bullet->set_id(new_id);

    bullets_.push_back(bullet);
}

void Enemy::update_bullets() {
    bullets_.erase(
        std::remove_if(
            bullets_.begin(),
            bullets_.end(),
            [] (std::shared_ptr<Bullet> bullet) {
                bool expr1 = bullet->is_dead();
                bool expr2 = bullet->get_health() < 1;

                return expr1 || expr2;
            }
        ),
        bullets_.end()
    );

    for (auto bullet : bullets_)
        bullet->update();
}

std::vector<std::shared_ptr<Bullet>>& Enemy::get_bullets() {
    return bullets_;
}
