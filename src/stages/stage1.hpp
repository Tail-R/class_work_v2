#pragma once

#include "../shared.hpp"
#include "stage.hpp"
#include "../sprites/player.hpp"
#include "../sprites/finals/meiling.hpp"
#include "../socket/socket.hpp"

class Player;

class Stage1 : Stage {
public:
    Stage1(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory);
    ~Stage1();

    StageTag start() override;

    std::atomic<bool> socket_is_ready_{false};

private:
    void spawn_worker() override;

    std::unique_ptr<Socket> sock_{nullptr};
    std::unique_ptr<std::thread> t_{nullptr};

    std::shared_ptr<Player> player_;
    std::shared_ptr<Meiling> meiling_;
};
