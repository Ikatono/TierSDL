#include "graphics.hpp"

namespace Graphics
{
    void Drawable::setHidden(bool hidden)
    {
        this->hidden = hidden;
    }
    bool Drawable::getHidden() const
    {
        return hidden;
    }
    //this is an extremely hacky way to do this
    Drawing Drawable::toDrawing(SDL_Renderer* renderer)
    {
        (void)SDL_GetError();
        const auto size = getSize();
        auto tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, size.w(), size.h());
        auto tempTex = SDL_GetRenderTarget(renderer);
        if (tex == nullptr)
            throw(SDL_GetError());
        if (SDL_SetRenderTarget(renderer, tex))
            throw(SDL_GetError());
        const auto tempHidden = getHidden();
        const auto tempCorner = getCorner();
        setHidden(false);
        setCorner({0, 0});
        draw(renderer);
        setCorner(tempCorner);
        setHidden(tempHidden);
        SDL_SetRenderTarget(renderer, tempTex);
        return tex;
    }
    Drawing::Drawing()
        : texture(nullptr)
    {

    }
    Drawing::Drawing(SDL_Texture* texture)
        : texture(texture)
    {

    }
    Drawing::Drawing(Drawing&& other)
        : texture(other.texture)
    {
        other.texture = nullptr;
    }
    Drawing& Drawing::operator=(Drawing&& other)
    {
        texture = other.texture;
        other.texture = nullptr;
        return *this;
    }
    Drawing::~Drawing()
    {
        if (texture != nullptr)
            SDL_DestroyTexture(texture);
    }
    FSize Drawing::getSize() const
    {
        FSize size;
        (void)SDL_GetError();
        if (SDL_GetTextureSize(texture, &size.w(), &size.h()))
            throw(SDL_GetError());
        return size;
    }
    void Drawing::drawAt(SDL_Renderer* renderer, FPoint point)
    {
        if (texture == nullptr)
            return;
        FRect rect = { point, getSize() };
        if (SDL_RenderTexture(renderer, texture, nullptr, &rect.inner))
            throw(SDL_GetError());
    }
}
