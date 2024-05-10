#include "stage1.hpp"

void tag_invoke(
    boost::json::value_from_tag,
    boost::json::value& jv,
    SDL_FRect const& region
    ) {

    jv = {
        {"x", region.x},
        {"y", region.y},
        {"w", region.w},
        {"h", region.h}
    };
}

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
    player_->set_stage1(this);
    player_->load_texture_from_path(PLAYER_TEXTURE);

    // Enemies
    enemies_ = std::make_shared<Enemies>();
}

Stage1::~Stage1() {
    if (t_ != nullptr)
        if (t_->joinable())
            t_->join();
}

void apply_damage_by_collision(
    std::shared_ptr<Player>,
    std::shared_ptr<Enemies>
    ) {
}

StageTag Stage1::start() {
    auto ctx = get_renderer();
    auto factory = get_factory();

    // Spawn a thread that observe the current event
    auto k = KeyEventListener();
    auto rx = k.activate();

    // Glue above thread to the player object
    player_->subscribe(std::move(rx));

    // Wait until the client to connect
    auto sock = Socket();
    sock.accept_client();

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
        enemies_->update();
        enemies_->unlock();

        // Send data via socket
        player_->lock();
        auto region = player_->get_region();
        player_->unlock();

        auto data = boost::json::value_from(region);
        auto data_str = boost::json::serialize(data);
        sock.send_data(data_str);

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
        auto enemy = std::make_shared<Enemy>(ctx, factory);

        SDL_Delay(1000);

        debug_log("DEBUG: Stage 1 worker thread has been successfully terminated");
    });

    if (t_ != nullptr)
    {
        thread.swap(*t_);
    }
    else
    {
        debug_log("FATAL: Failed to spawn a stage 1 worker thread");
    }
}
