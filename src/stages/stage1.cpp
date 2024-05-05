#include "stage1.hpp"

Stage1::Stage1(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : Stage{ctx, factory} {}

StageTag Stage1::start() {
    auto ctx = get_renderer();
    auto factory = get_factory();

    player_ = std::make_unique<Player>(ctx, factory);

    player_->set_region(
        (WINDOW_WIDTH / 2) + (PLAYER_WIDTH / 2),
        WINDOW_HEIGHT - (PLAYER_HEIGHT * 2),
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    );

    player_->set_base_speed(10.0);
    player_->set_bullet_dx(0.0);
    player_->set_bullet_dy(-20.0);
    player_->set_bullet_rate(6);
    player_->set_bullet_health(1);
    player_->load_texture_from_path(PLAYER_TEXTURE);

    /*
    Spawn a thread that observe the current event
     */
    auto k = KeyEventListener();
    auto rx = k.activate();

    /*
    Glue above thread to the player object
     */
    player_->subscribe(std::move(rx));

    auto prev_ticks = SDL_GetTicks();

    while (!player_->quit.load()) { // ask player to exit the loop
        SDL_RenderClear(ctx);

        player_->update();

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
}
