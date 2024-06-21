#include "tiercell.hpp"
#include "text.hpp"
#include "window.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

namespace Tier
{
    Cell::Cell(FRect rect)
        : rect(rect), text(" "), font(nullptr)
    { }
    Cell::Cell(FPoint point, FSize size)
        : rect(point, size), text(" "), font(nullptr)
    { }
    Color Cell::getBackgroundColor() const { return backgroundColor; }
    void Cell::setBackgroundColor(Color color)
    {
        backgroundColor = color;
    }
    Color Cell::getFontColor() const { return fontColor; }
    void Cell::setFontColor(Color color)
    {
        fontColor = color;
        updateFontTexture();
    }
    void Cell::setText(std::string newText)
    {
        text = newText;
        updateFontTexture();
    }
    const std::string* Cell::peekText() const
    {
        return &text;
    }
    void Cell::draw(SDL_Renderer* renderer) const
    {
        if (hidden)
            return;
        Color oldColor;
        SDL_GetRenderDrawColor(renderer, &oldColor.r(), &oldColor.g(), &oldColor.b(), &oldColor.a());
        SDL_SetRenderDrawColor(renderer, backgroundColor.r(), backgroundColor.g(), backgroundColor.b(), backgroundColor.a());
        if (SDL_RenderFillRect(renderer, &rect.inner))
        {
            throw(SDL_GetError());
        }
        SDL_SetRenderDrawColor(renderer, oldColor.r(), oldColor.g(), oldColor.b(), oldColor.a());
        // auto* texture = Text::renderText(text.c_str(), fontColor, rect, renderer);
        if (fontTexture != nullptr)
        {
            FRect dst = FRect(rect.corner(), fontTextureSize);
            SDL_RenderTexture(renderer, fontTexture, nullptr, &dst.inner);
        }
        // SDL_DestroyTexture(texture);
    }
    void Cell::moveBy(float x, float y)
    {
        rect.x() += x;
        rect.y() += y;
    }
    void Cell::moveTo(FPoint point)
    {
        rect.x() = point.x();
        rect.y() = point.y();
    }
    bool Cell::contains(Point point) const { return rect.contains(point); }
    bool Cell::contains(FPoint point) const { return rect.contains(point); }
    void Cell::updateFontTexture()
    {
        if (!font || fontSize <= 0)
        {
            fontTexture = nullptr;
            return;
        }
        SDL_GetError();
        fontTexture = Text::renderText(text.c_str(), font.get(), fontColor, rect, Window::getRenderer());
        if (!fontTexture)
        {
            auto err = SDL_GetError();
            std::cerr << "Font render error: " << err << std::endl;
            throw(err);
        }
        SDL_SetTextureAlphaMod(fontTexture, fontColor.a());
        SDL_SetTextureBlendMode(fontTexture, SDL_BLENDMODE_BLEND);
        SDL_BlendMode bmode;
        if (SDL_GetTextureBlendMode(fontTexture, &bmode))
        {
            auto err = SDL_GetError();
            std::cerr << "Failed to get blend mode: " << err << std::endl;
            throw(err);
        }
        // std::cout << "Font Blend Mode: " << bmode << std::endl;
        SDL_GetTextureSize(fontTexture, &fontTextureSize.w(), &fontTextureSize.h());
        // FSize size;
        // SDL_GetTextureSize(fontTexture, &size.w, &size.h);
        // std::cout << "Font Texture Size: " << size.w << ", " << size.h << std::endl;
    }
    FPoint Cell::getCorner() const
    {
        return rect.corner();
    }
    FSize Cell::getSize() const
    {
        return rect.size();
    }
    void Cell::setCorner(FPoint point)
    {
        rect.x() = point.x();
        rect.y() = point.y();
    }
    int Cell::getFontSize() const
    {
        return fontSize;
    }
    void Cell::setFontSize(int ptsize)
    {
        fontSize = ptsize;
        font = Text::getFont(ptsize);
        updateFontTexture();
    }
}
