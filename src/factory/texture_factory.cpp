#include "texture_factory.hpp"

TextureFactory::TextureFactory(SDL_Renderer* ctx) : ctx_{ctx} {}
TextureFactory::~TextureFactory() {
    for (auto texture : stock_) {
        if (texture.second != nullptr)
            SDL_DestroyTexture(texture.second);
    }
}

SDL_Texture* TextureFactory::create_texture_from_path(std::string_view path) {
    if (stock_.count(path) == 0)
    {
        SDL_Surface* loaded_surface = IMG_Load(std::string(path).c_str());

        if (loaded_surface == nullptr)
        {
            debug_log("ERROR: Failed to create surface");
            return nullptr;
        }
        else
        {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx_, loaded_surface);
            SDL_FreeSurface(loaded_surface);

            if (texture == nullptr)
            {
                debug_log("ERROR: Failed to create texture");
                return nullptr;
            }
            else
            {
                // debug_log("DEBUG: Texture has been created");
                stock_[path] = texture;

                return texture;
            }
        }
    }
    else
    {
        // debug_log("DEBUG: Cached texture founded");
        return stock_.find(path)->second;
    }
}
