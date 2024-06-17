#pragma once

#include "../shared.hpp"
#include "../factory/texture_factory.hpp"

class Sprite {
public:
    Sprite(SDL_Renderer* ctx, std::shared_ptr<TextureFactory>);

    void set_region(float x, float y, float w, float h);
    void set_id(unsigned int id);
    void set_x(float x);
    void set_y(float y);
    void set_w(float w);
    void set_h(float h);
    void set_angle(double deg);
    void set_texture_path(std::string_view texture_path);
    void update_texture();
    void load_texture_from_path(std::string_view path);

    void render();
    virtual void update();

    TaggedRegion get_tregion();
    const SDL_FRect& get_region();
    unsigned int get_id();
    float get_x();
    float get_y();
    float get_w();
    float get_h();
    double get_angle();
    SDL_Renderer* get_renderer();
    std::shared_ptr<TextureFactory> get_factory();

private:
    unsigned int id_{0};
    SDL_Renderer* ctx_;
    SDL_Texture* texture_{nullptr};
    std::string_view texture_path_{""};
    SDL_FRect region_{0.0, 0.0, 0.0, 0.0};
    double angle_{0.0};

    std::shared_ptr<TextureFactory> factory_;
};
