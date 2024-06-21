#include "tierrow.hpp"
#include "window.hpp"
#include <SDL3/SDL_assert.h>
#include <algorithm>
#include <iostream>

namespace Tier
{
    Row::Row()
    {

    }
    const std::string* Row::getText() const
    {
        return &text;
    }
    void Row::setText(std::string text)
    {
        this->text = text;
        updateFontTexture();
    }
    Color Row::getBackgroundColor() const
    {
        return backgroundColor;
    }
    void Row::setBackgroundColor(Color color)
    {
        backgroundColor = color;
    }
    Color Row::getTextColor() const
    {
        return textColor;
    }
    void Row::setTextColor(Color color)
    {
        textColor = color;
        updateFontTexture();
    }
    int Row::getTextSize() const
    {
        return textSize;
    }
    void Row::setTextSize(int ptsize)
    {
        textSize = ptsize;
        updateFontTexture();
    }
    void Row::draw(SDL_Renderer* renderer) const
    {
        if (hidden)
            return;
        for (auto& child : children)
        {
            child->draw(renderer);
        }
    }
    FRect Row::getRect() const
    {
        return {
            corner.x(),
            corner.y(),
            static_cast<float>(Window::getWindowSize().w()),
            neededHeight,
        };
    }
    FPoint Row::getCorner() const
    {
        return corner;
    }
    void Row::setCorner(FPoint point)
    {
        corner = point;
        arrangeChildren();
    }
    void Row::addChild(Cell* child, int index)
    {
        if (index < 0)
            index = children.size() + index + 1;
        SDL_assert(index >= 0 && index <= children.size());
        children.insert(children.begin() + index, child);
        arrangeChildren();
    }
    void Row::resizeCells(FSize size)
    {
        arrangeChildren();
    }
    void Row::removeChild(Cell* child)
    {
        auto it = std::find(children.begin(), children.end(), child);
        arrangeChildren();
    }
    float Row::getNeededHeight() const
    {
        return neededHeight;
    }
    void Row::updateFontTexture()
    {
        
    }
    //use a flow algorithm
    //could be simplified since all cells are the same size
    void Row::arrangeChildren()
    {
        std::cout << "Arrange Children" << std::endl;
        int width = Window::getWindowSize().w();
        FPoint point = getCorner();
        float maxHeight = 0;
        for (auto child : children)
        {
            const auto size = child->rect.size();
            if (point.x() + size.w() <= width)
            {
                child->moveTo(point);
                maxHeight = std::max(maxHeight, size.h());
                point.x() += size.w();
            }
            //new row
            else
            {
                point.x() = 0;
                point.y() = point.y() + maxHeight;
                maxHeight = 0;
            }
        }
        neededHeight = point.y() + maxHeight;
    }
    bool Row::contains(Point point) const
    {
        return getRect().contains(point);
    }
    bool Row::contains(FPoint point) const
    {
        return getRect().contains(point);
    }
    FSize Row::getSize() const
    {
        return getRect().size();
    }
}
