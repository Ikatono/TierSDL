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
        TRY(tex == nullptr);
        TRY(SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND));
        auto tempTex = SDL_GetRenderTarget(renderer);
        TRY(SDL_SetRenderTarget(renderer, tex));
        const auto tempHidden = getHidden();
        const auto tempCorner = getCorner();
        setHidden(false);
        setCorner({0, 0});
        draw(renderer);
        setCorner(tempCorner);
        setHidden(tempHidden);
        TRY(SDL_SetRenderTarget(renderer, tempTex));
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
        if (texture != nullptr)
            SDL_DestroyTexture(texture);
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
        TRY(SDL_GetTextureSize(texture, &size.w(), &size.h()));
        return size;
    }
    void Drawing::drawAt(SDL_Renderer* renderer, FPoint point, float scale) const
    {
        if (texture == nullptr)
            return;
        if (renderer == nullptr)
            throw("Drawing::drawAt() renderer is nullptr");
        FRect rect = { point, getSize() * scale };
        TRY(SDL_RenderTexture(renderer, texture, nullptr, &rect.inner));
    }
    Drawing::operator bool() const
    {
        return texture != nullptr;
    }
}
