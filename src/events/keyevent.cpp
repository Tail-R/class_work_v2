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
