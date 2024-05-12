#pragma once

#include "../shared.hpp"
#include "enemy.hpp"

class Enemies {
public:
    void lock();
    void unlock();
    void update();

    std::vector<std::shared_ptr<Enemy>> sprites_;

private:
    std::mutex mtx_;
};
