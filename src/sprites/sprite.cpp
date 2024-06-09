#include "sprite.hpp"

Sprite::Sprite(SDL_Renderer* ctx, std::shared_ptr<TextureFactory> factory)
    : ctx_{ctx}
    , factory_{factory} {}

void Sprite::set_region(float x, float y, float w, float h) {
    region_.x = x;
    region_.y = y;
    region_.w = w;
    region_.h = h;
}

void Sprite::set_x(float x) { region_.x = x; }
void Sprite::set_y(float y) { region_.y = y; }
void Sprite::set_w(float w) { region_.w = w; }
void Sprite::set_h(float h) { region_.h = h; }
void Sprite::set_angle(double deg) { angle_ = deg; }
void Sprite::set_id(unsigned int id) { id_ = id; }

void Sprite::set_texture_path(std::string_view texture_path) {
    texture_path_ = texture_path;
}

void Sprite::update_texture() {
    if (texture_path_ != "")
    {
        texture_ = factory_->create_texture_from_path(texture_path_);
    }
    else
    {
        debug_log("WARN: Texture path is not set");
    }
}

void Sprite::load_texture_from_path(std::string_view path) {
    texture_ = factory_->create_texture_from_path(path);
}

void Sprite::render() {
    SDL_RenderCopyExF(
        ctx_,
        texture_,
        nullptr,
        &region_,
        angle_,
        nullptr,
        SDL_FLIP_NONE
    );
}

void Sprite::update() { render(); }

const SDL_FRect& Sprite::get_region() { return region_; }

float Sprite::get_x() { return region_.x; }
float Sprite::get_y() { return region_.y; }
float Sprite::get_w() { return region_.w; }
float Sprite::get_h() { return region_.h; }
double Sprite::get_angle() { return angle_; }
unsigned int Sprite::get_id() { return id_; }

SDL_Renderer* Sprite::get_renderer() { return ctx_; }
std::shared_ptr<TextureFactory> Sprite::get_factory() { return factory_; }
