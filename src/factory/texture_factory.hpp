#pragma once

#include "../shared.hpp"

class TextureFactory {
public:
    TextureFactory(SDL_Renderer* ctx);
    ~TextureFactory();

    TextureFactory(TextureFactory const&) = delete;
    TextureFactory(TextureFactory&&) = default;
    TextureFactory& operator = (TextureFactory const&) = delete;
    TextureFactory& operator = (TextureFactory&&) = default;

    SDL_Texture* create_texture_from_path(std::string_view path);

private:
    SDL_Renderer* ctx_;
    std::unordered_map<std::string_view, SDL_Texture*> stock_;
};
