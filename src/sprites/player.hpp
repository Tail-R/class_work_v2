#pragma once

#include "sprite.hpp"
#include "bullet.hpp"
#include "../events/keyevent.hpp"

class Player : public Sprite {
public:
    Player(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);
    ~Player();

    void set_dx(float dx);
    void set_dy(float dy);
    void set_base_speed(float base_speed);
    void set_bullet_dx(float bullet_dx);
    void set_bullet_dy(float bullet_dy);
    void set_bullet_rate(int bullet_rate);
    void set_bullet_health(int bullet_health);

    void lock();
    void unlock();
    void update() override;
    bool collided_with(std::shared_ptr<Sprite> sprite);

    void subscribe(Channel<std::shared_ptr<KBState>> receiver);
    void unsubscribe();

    std::vector<std::shared_ptr<Bullet>>& get_bullets();

    std::atomic<bool> quit_{false};

private:
    void move();
    void fire();
    void update_bullets();

    std::mutex mtx_;

    std::unique_ptr<std::thread> t_{nullptr};
    std::unique_ptr<Channel<std::shared_ptr<KBState>>> kbstate_rx_{nullptr};

    float dx_{0.0};
    float dy_{0.0};
    float base_speed_{0.0};

    std::vector<std::shared_ptr<Bullet>> bullets_;
    bool firing_{false};
    float bullet_dx_{0.0};
    float bullet_dy_{0.0};
    int bullet_health_{0};
    int bullet_timer_{0};
    int bullet_rate_{0};
};
