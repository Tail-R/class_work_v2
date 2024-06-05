#include "player.hpp"

Player::Player(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : Sprite{ctx, factory} {}

Player::~Player() {
    if (t_ != nullptr)
        if (t_->joinable())
            t_->join();

    debug_log("DEBUG: Player has been destructed");
}

void Player::set_dx(float dx) { dx_ = dx; }
void Player::set_dy(float dy) { dy_ = dy; }
void Player::set_base_speed(float base_speed) { base_speed_ = base_speed; }
void Player::set_bullet_dx(float bullet_dx) { bullet_dx_ = bullet_dx; }
void Player::set_bullet_dy(float bullet_dy) { bullet_dy_ = bullet_dy; }
void Player::set_bullet_rate(int bullet_rate) { bullet_rate_ = bullet_rate; }
void Player::set_bullet_health(int bullet_health) { bullet_health_ = bullet_health; }
void Player::set_stage1(Stage1* stage1) { stage1_ = stage1; }

void Player::lock() { mtx_.lock(); }
void Player::unlock() { mtx_.unlock(); }

void Player::update() {
    move();

    if (firing_ && (bullet_timer_ == 0))
    {
        fire();
        bullet_timer_ = bullet_rate_;
    }
    else
    {
        bullet_timer_ = bullet_timer_ > 0 ? bullet_timer_ - 1 : 0;
    }

    update_bullets();

    render();
}

bool Player::collided_with(std::shared_ptr<Sprite> sprite) {
    auto s1 = get_region();
    auto s2 = sprite->get_region();

    bool expr1 = (s1.x <= s2.x + s2.w) && (s2.x <= s1.x + s1.w);
    bool expr2 = (s1.y <= s2.y + s2.h) && (s2.y <= s1.y + s1.h);

    return expr1 && expr2;
}

void Player::subscribe(Channel<std::shared_ptr<KBState>> receiver) {
    if (t_ != nullptr)
        if (t_->joinable())
            t_->detach();

    t_ = std::make_unique<std::thread>();
    kbstate_rx_ = std::make_unique<Channel<std::shared_ptr<KBState>>>(std::move(receiver));

    std::thread thread([this]{
        debug_log("DEBUG: Player update thread has been spawned");

        for (;;) {
            if (kbstate_rx_ == nullptr)
            {
                return;
            }
            else if (auto keys = kbstate_rx_->recv(); keys != std::nullopt)
            {
                lock();

                if ((*keys)->up == Key::PRESSED)
                {
                    dy_ = -base_speed_;
                }
                else if ((*keys)->down == Key::PRESSED)
                {
                    dy_ = base_speed_;
                }
                else
                {
                    dy_ = 0.0;
                }

                if ((*keys)->left == Key::PRESSED)
                {
                    dx_ = -base_speed_;
                }
                else if ((*keys)->right == Key::PRESSED)
                {
                    dx_ = base_speed_;
                }
                else
                {
                    dx_ = 0.0;
                }

                if ((*keys)->space == Key::PRESSED)
                {
                    firing_ = true;
                }
                else
                {
                    firing_ = false;
                }

                unlock();

                if ((*keys)->QUIT) {
                    quit_.store(true);
                    return;
                }
            }
            else
            {
                return;
            }
        }
    });

    if (t_ != nullptr)
    {
        thread.swap(*t_);
    }
    else
    {
        debug_log("FATAL: Failed to spawn a player update thread");
    }
}

void Player::unsubscribe() {
    if (kbstate_rx_ != nullptr)
        kbstate_rx_->close();
}

void Player::move() {
    auto region = get_region();

    auto next_x = region.x + dx_;
    auto next_y = region.y + dy_;

    bool expr1 = (next_x >= 0) && (WINDOW_WIDTH - region.w >= next_x);
    bool expr2 = (next_y >= 0) && (WINDOW_HEIGHT - region.h >= next_y);

    if (expr1 && expr2) {
        set_x(next_x);
        set_y(next_y);
    }
}

void Player::fire() {
    auto ctx = get_renderer();
    auto factory = get_factory();
    auto region = get_region();
    auto bullet = std::make_shared<Bullet>(ctx, factory);

    bullet->set_region(
        region.x,
        region.y,
        PLAYER_BULLET_WIDTH,
        PLAYER_BULLET_HEIGHT
    );

    bullet->set_dx(bullet_dx_);
    bullet->set_dy(bullet_dy_);
    bullet->set_health(bullet_health_);
    bullet->set_texture_path(PLAYER_BULLET_TEXTURE);

    bullets_.push_back(bullet);
}

void Player::update_bullets() {
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

std::vector<std::shared_ptr<Bullet>>& Player::get_bullets() {
    return bullets_;
}
