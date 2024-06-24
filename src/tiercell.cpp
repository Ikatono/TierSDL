#include "tiercell.hpp"
#include "text.hpp"
#include "window.hpp"
#include "image.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <limits>

namespace Tier
{
    //0,0 size should be ok because it resizes before drawing
    Cell::Cell() : id(getNewId())
    {

    }
    Cell::~Cell()
    {
        cellIds.erase(id);
    }
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
        TRY(SDL_GetRenderDrawColor(renderer, &oldColor.r(), &oldColor.g(), &oldColor.b(), &oldColor.a()));
        TRY(SDL_SetRenderDrawColor(renderer, backgroundColor.r(), backgroundColor.g(), backgroundColor.b(), backgroundColor.a()));
        const FRect rect = getRect();
        TRY(SDL_RenderFillRect(renderer, &rect.inner));
        TRY(SDL_SetRenderDrawColor(renderer, oldColor.r(), oldColor.g(), oldColor.b(), oldColor.a()));
        if (image)
        {
            const auto size = rect.size();
            const auto imageSize = image.getSize();
            const auto scaleV = size.w() / imageSize.w();
            const auto scaleH = size.h() / imageSize.h();
            if (scaleV <= scaleH)
            {
                image.drawAt(renderer, {corner.x(), corner.y() + (size.h() - imageSize.h() * scaleV) / 2 }, scaleV);
            }
            else
            {
                image.drawAt(renderer, {corner.x() + (size.w() - imageSize.w() * scaleH) / 2, corner.y()}, scaleH);
                // image.drawAt(renderer, corner, scale);
            }
        }
        // auto* texture = Text::renderText(text.c_str(), fontColor, rect, renderer);
        if (fontTexture)
        {
            FRect dst = FRect(rect.corner(), fontTexture.getSize());
            // SDL_RenderTexture(renderer, fontTexture, nullptr, &dst.inner);
            fontTexture.drawAt(renderer, rect.corner());
        }
        // SDL_DestroyTexture(texture);
    }
    void Cell::moveBy(float x, float y)
    {
        corner.x() += x;
        corner.y() += y;
    }
    void Cell::moveTo(FPoint point)
    {
        corner.x() = point.x();
        corner.y() = point.y();
    }
    bool Cell::contains(Point point) const { return getRect().contains(point); }
    bool Cell::contains(FPoint point) const { return getRect().contains(point); }
    void Cell::updateFontTexture()
    {
        if (!font || fontSize <= 0)
        {
            fontTexture = nullptr;
            return;
        }
        SDL_GetError();
        FRect rect = getRect();
        fontTexture = Text::renderText(text.c_str(), font.get(), fontColor, rect, Window::getRenderer());
        if (!fontTexture)
        {
            auto err = SDL_GetError();
            std::cerr << "Font render error: " << err << std::endl;
            throw(err);
        }
    }
    FPoint Cell::getCorner() const
    {
        return corner;
    }
    FSize Cell::getSize() const
    {
        return size;
    }
    void Cell::setSize(FSize size)
    {
        Cell::size.w() = size.w();
        Cell::size.h() = size.h();
    }
    void Cell::setCorner(FPoint point)
    {
        corner.x() = point.x();
        corner.y() = point.y();
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
    void Cell::setImage(const char* filepath)
    {
        image = Graphics::loadImage(filepath);
    }
    uint32_t Cell::getId() const
    {
        return id;
    }
    std::unordered_set<uint32_t> Cell::cellIds;
    uint32_t Cell::getNewId()
    {
        for (uint32_t i = 1; i < std::numeric_limits<uint32_t>::max(); i++)
        {
            if (!cellIds.contains(i))
            {
                cellIds.insert(i);
                return i;
            }
        }
        throw("Could not create cell id");
    }
    void Cell::updateFont()
    {
        throw("Cell::updateFont not implemented");
    }
    FSize Cell::size = {100, 100};
    FSize Cell::cellSize()
    {
        return Cell::size;
    }
    FRect Cell::getRect() const
    {
        return { corner, Cell::size };
    }
}
