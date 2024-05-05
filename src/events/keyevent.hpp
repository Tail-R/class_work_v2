#pragma once

#include "../shared.hpp"
#include "../channel.hpp"

typedef enum {
    RELEASED,
    PRESSED
} Key;

typedef struct {
    bool QUIT{false};

    Key up{Key::RELEASED};
    Key down{Key::RELEASED};
    Key left{Key::RELEASED};
    Key right{Key::RELEASED};
    Key space{Key::RELEASED};
    Key escape{Key::RELEASED};
} KBState;

class KeyEventListener {
public:
    ~KeyEventListener();

    Channel<std::shared_ptr<KBState>> activate();

private:
    std::unique_ptr<std::thread> t_{nullptr};
    std::shared_ptr<KBState> kbstate_{nullptr};
};
