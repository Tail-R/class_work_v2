#include "app.hpp"

App::App() {
    if (!prelude()) {
        debug_log("FATAL: Failed to initialize the application");

        byebye();
        exit(1);
    }

    factory_ = std::make_shared<TextureFactory>(ctx_);
}

App::~App() { byebye(); }

bool App::prelude() {
    bool success = true;

    /*
    Initialize the video subsystem,
    Establish a connection to the window manager
     */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        success = false;
        debug_log("FATAL: Failed to initialize the SDL video subsystem");
    }
    else
    {
        debug_log("DEBUG: SDL video subsystem has been initialized");

        // Set texture filtering following to the "nearest" or "liner"
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"))
            debug_log("WARN: Linear texture filtering is not enabled");

        auto window_flags = SDL_WINDOW_HIDDEN;

        window_ = SDL_CreateWindow(
            std::string(WINDOW_TITLE).c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            window_flags
        );

        if (window_ == nullptr)
        {
            success = false;
            debug_log("FATAL: Failed to create window");
        }
        else
        {
            debug_log("DEBUG: Window has been successfully created");

            ctx_ = SDL_CreateRenderer(
                window_,
                /*
                The index of the rendering driver to initialize,
                or -1 to initialize the first one supporting the requested flags
                 */
                -1,
                RENDERER_FLAGS
            );

            if (ctx_ == nullptr)
            {
                success = false;
                debug_log("FATAL: Failed to create renderer");
            }
            else
            {
                debug_log("DEBUG: Renderer has been successfully created");

                SDL_SetRenderDrawColor(ctx_, 0, 0, 0, 255);

                if (IMG_Init(IMG_FLAGS) != IMG_FLAGS)
                {
                    success = false;
                    debug_log("FATAL: Failed to initialize the SDL image subsystem");
                }
                else
                {
                    debug_log("DEBUG: SDL image subsystem has been initialized");
                }
            }
        }
    }

    return success;
}

void App::run() {
    SDL_ShowWindow(window_);

    auto s1 = Stage1(ctx_, factory_);
    s1.start();
}

void App::byebye() {
    SDL_HideWindow(window_);

    if (ctx_ != nullptr) {
        SDL_DestroyRenderer(ctx_);
        ctx_ = nullptr;
    }

    if (window_ != nullptr) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    debug_log("DEBUG: Deallocating application resources");

    IMG_Quit();
    SDL_Quit();
}
