#include "keyevent.hpp"

KeyEventListener::~KeyEventListener() {
    if (t_ != nullptr)
        if (t_->joinable())
            t_->join();
}

Channel<std::shared_ptr<KBState>> KeyEventListener::activate() {
    if (t_ != nullptr)
        if (t_->joinable())
            t_->detach();

    t_ = std::make_unique<std::thread>();

    auto [sender, receiver] = channel<std::shared_ptr<KBState>>();

    kbstate_ = std::make_shared<KBState>();

    std::thread thread([this](Channel<std::shared_ptr<KBState>> sender) {
        debug_log("DEBUG: Keyboard event thread has been spawned");

        SDL_Event event;

        while (SDL_WaitEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    kbstate_->QUIT = true;
                    sender.send(kbstate_);
                    return;
                case SDL_KEYDOWN:
                    if (event.key.repeat == 0) {
                        switch (event.key.keysym.scancode) {
                            case SDL_SCANCODE_UP: kbstate_->up = Key::PRESSED; break;
                            case SDL_SCANCODE_DOWN: kbstate_->down = Key::PRESSED; break;
                            case SDL_SCANCODE_LEFT: kbstate_->left = Key::PRESSED; break;
                            case SDL_SCANCODE_RIGHT: kbstate_->right = Key::PRESSED; break;
                            case SDL_SCANCODE_SPACE: kbstate_->space = Key::PRESSED; break;
                            case SDL_SCANCODE_ESCAPE: kbstate_->escape = Key::PRESSED; break;
                            default: break;
                        }
                        if (!sender.send(kbstate_)) return;
                    }
                    break;
                case SDL_KEYUP:
                    if (event.key.repeat == 0) {
                        switch (event.key.keysym.scancode) {
                            case SDL_SCANCODE_UP: kbstate_->up = Key::RELEASED; break;
                            case SDL_SCANCODE_DOWN: kbstate_->down = Key::RELEASED; break;
                            case SDL_SCANCODE_LEFT: kbstate_->left = Key::RELEASED; break;
                            case SDL_SCANCODE_RIGHT: kbstate_->right = Key::RELEASED; break;
                            case SDL_SCANCODE_SPACE: kbstate_->space = Key::RELEASED; break;
                            case SDL_SCANCODE_ESCAPE: kbstate_->escape = Key::RELEASED; break;
                            default: break;
                        }
                        if (!sender.send(kbstate_)) return;
                    }
                    break;
                default:
                    break;
            }
        }
    }, std::move(sender));

    if (t_ != nullptr)
    {
        thread.swap(*t_);
    }
    else
    {
        debug_log("FATAL: Failed to spawn a keyboard event thread");
    }

    return std::move(receiver);
}

Channel<std::shared_ptr<KBState>> KeyEventListener::activate_socket() {
    if (t_ != nullptr)
        if (t_->joinable())
            t_->detach();

    // Socket
    auto socket_name = std::string(KEY_EVENT_SOCKET_NAME);
    sock_ = std::make_unique<ClientSocket>(socket_name);

    t_ = std::make_unique<std::thread>();

    auto [sender, receiver] = channel<std::shared_ptr<KBState>>();

    kbstate_ = std::make_shared<KBState>();

    std::thread thread([this](Channel<std::shared_ptr<KBState>> sender) {
        debug_log("DEBUG: Keyboard event thread has been spawned");

        for (;;) {
            auto raw_data = sock_->recv_data();

            if (raw_data == std::nullopt) {
                debug_log("DEBUG: Exiting keyboard event thread");
                return;
            }

            auto kdata = *(*raw_data).c_str();

            kbstate_->up    = kdata & 0b00000001 ? Key::PRESSED : Key::RELEASED;
            kbstate_->down  = kdata & 0b00000010 ? Key::PRESSED : Key::RELEASED;
            kbstate_->left  = kdata & 0b00000100 ? Key::PRESSED : Key::RELEASED;
            kbstate_->right = kdata & 0b00001000 ? Key::PRESSED : Key::RELEASED;
            kbstate_->QUIT  = kdata & 0b10000000 ? Key::PRESSED : Key::RELEASED;

            if (!sender.send(kbstate_))
                return;
        }
    }, std::move(sender));

    if (t_ != nullptr)
    {
        thread.swap(*t_);
    }
    else
    {
        debug_log("FATAL: Failed to spawn a keyboard event thread");
    }

    return std::move(receiver);
}
