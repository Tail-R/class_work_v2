#include "stage1.hpp"

Stage1::Stage1(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : Stage{ctx, factory} {

    // Player
    player_ = std::make_unique<Player>(ctx, factory);

    player_->set_region(
        (WINDOW_WIDTH / 2) + (PLAYER_WIDTH / 2),
        WINDOW_HEIGHT - (PLAYER_HEIGHT * 2),
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    );

    player_->set_base_speed(PLAYER_BASE_SPEED);
    player_->set_bullet_dx(PLAYER_BULLET_DX);
    player_->set_bullet_dy(PLAYER_BULLET_DY);
    player_->set_bullet_rate(PLAYER_BULLET_RATE);
    player_->set_bullet_health(PLAYER_BULLET_HEALTH);
    player_->load_texture_from_path(PLAYER_TEXTURE);

    // Enemies
    enemies_ = std::make_shared<Enemies>();
}

Stage1::~Stage1() {
    if (t_ != nullptr)
        if (t_->joinable())
            t_->join();
}

StageTag Stage1::start() {
    auto ctx = get_renderer();
    auto factory = get_factory();

    // Spawn a thread that observe the current event
    auto k = KeyEventListener();
    auto rx = k.activate();

    // Glue above thread to the player object
    player_->subscribe(std::move(rx));

    auto prev_ticks = SDL_GetTicks();

    spawn_worker(); // Invoke the worker thread

    for (;;) {
        if (player_->quit_.load()) { // ask player to exit the loop
            quit_.store(true);
            break;
        }

        SDL_RenderClear(ctx);

        player_->lock();
        player_->update();
        player_->unlock();

        enemies_->lock();
        auto& sprites = enemies_->get_sprites();

        for (auto enemy : sprites) {
            auto& bullets = enemy->get_bullets();

            for (auto bullet : bullets) {
                player_-> lock();

                if (player_->collided_with(bullet)) {
                    bullet->set_health(0);
                    player_->set_x(
                        (WINDOW_WIDTH / 2) + (PLAYER_WIDTH / 2)
                    );
                    player_->set_y(
                        WINDOW_HEIGHT - (PLAYER_HEIGHT * 2)
                    );
                }

                player_->unlock();
            }
        }

        enemies_->update();
        enemies_->unlock();

        // Calculate the delay manually
        auto current_ticks = SDL_GetTicks();
        auto elapsed = current_ticks - prev_ticks;

        if (elapsed < WINDOW_FRAME_LENGTH)
            SDL_Delay(WINDOW_FRAME_LENGTH - elapsed);

        prev_ticks = current_ticks;

        SDL_RenderPresent(ctx);
    }

    /*
    Don't forget to unsubscribe thread, player object's destructor will
    wait the join of the subscribed thread so this program will freeze.
     */
    player_->unsubscribe();

    return StageTag::EXIT;
}

void Stage1::spawn_worker() {
    if (t_ != nullptr)
        if (t_->joinable())
            t_->detach();

    t_ = std::make_unique<std::thread>();

    std::thread thread([this]{
        debug_log("DEBUG: Stage 1 worker thread has been spawned");

        auto ctx = get_renderer();
        auto factory = get_factory();

        /* Phase 1 */
        auto enemy = std::make_shared<Enemy>(ctx, factory);

        enemy->set_region(
            (WINDOW_WIDTH / 2) + (ENEMY_WIDTH / 2),
            ENEMY_HEIGHT,
            ENEMY_WIDTH,
            ENEMY_HEIGHT
        );

        enemy->set_dx(0.0);
        enemy->set_dy(0.0);
        enemy->set_texture_path(ENEMY_TEXTURE);
        enemy->set_bullet_health(1);

        enemies_->lock();
        auto& sprites = enemies_->get_sprites();
        sprites.push_back(enemy);
        enemies_->unlock();

        // circle shot
        auto flames = 600;
        while (flames--) {
            if (quit_.load())
                return;

            if (flames % 60 == 0) {
                enemies_->lock();
                for (auto enemy : sprites) {
                    for (float i = 0, two_pi = M_PI * 2; i < two_pi; i+=two_pi / 20) {
                        enemy->set_bullet_dx(cos(i) * 1.2);
                        enemy->set_bullet_dy(sin(i) * 1.2);
                        enemy->fire();
                    }
                }

                enemies_->unlock();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        debug_log("DEBUG: Stage 1 worker thread end");
    });

    if (t_ != nullptr)
    {
        thread.swap(*t_);
    }
    else
    {
        debug_log("FATAL: Failed to spawn a worker thread in stage 1");
    }
}
