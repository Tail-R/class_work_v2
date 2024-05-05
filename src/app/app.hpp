#include "../shared.hpp"
#include "../factory/texture_factory.hpp"
#include "../events/keyevent.hpp"
#include "../stages/stage.hpp"
#include "../stages/stage1.hpp"

class App {
public:
    App();
    ~App();

    void run();

private:
    bool prelude();
    void byebye();

    SDL_Window* window_{nullptr};
    SDL_Renderer* ctx_{nullptr};
    std::shared_ptr<TextureFactory> factory_;
};
