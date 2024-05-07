#pragma once

#include "../shared.hpp"
#include "enemy.hpp"

class Enemies {
public:
    void lock();
    void unlock();
    void update();

    std::vector<std::shared_ptr<Enemy>>& get_sprites();

private:
    std::mutex mtx_;
    std::vector<std::shared_ptr<Enemy>> sprites_;
};
