#include "tierrow.hpp"
#include "window.hpp"
#include <SDL3/SDL_assert.h>
#include <algorithm>
#include <iostream>

namespace Tier
{
    Row::Row()
        : id(getNewId()), text(" ")
    {

    }
    Row::~Row()
    {
        rowIds.erase(id);
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
        return color;
    }
    void Row::setColor(Color color)
    {
        Row::color = color;
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
        updateFontObject();
        updateFontTexture();
    }
    void Row::updateFontObject()
    {
        font = Text::getFont(textSize);
    }
    void Row::updateFontTexture()
    {
        if (font == nullptr)
        {
            fontTexture = nullptr;
            return;
        }
        fontTexture = Text::renderText(text.c_str(), font.get(), textColor, getTitleRect(), Window::getRenderer());
        TRY(!fontTexture);
    }
    void Row::draw(SDL_Renderer* renderer) const
    {
        if (hidden)
            return;
        Color oldColor;
        Color bgColor = color.blendWith(WHITE);
        // std::cout << bgColor.r() << ", " << bgColor.g() << ", " << bgColor.b() << ", " << bgColor.a() << std::endl;
        // printf("%d, %d, %d, %d\n", bgColor.r(), bgColor.g(), bgColor.b(), bgColor.a());
        TRY(SDL_GetRenderDrawColor(renderer, &oldColor.r(), &oldColor.g(), &oldColor.b(), &oldColor.a()));
        TRY(SDL_SetRenderDrawColor(renderer, bgColor.r(), bgColor.g(), bgColor.b(), 255));
        FRect rect = getRect();
        TRY(SDL_RenderFillRect(renderer, &rect.inner));
        rect.w() = Cell::cellSize().w();
        rect.h() = Cell::cellSize().h();
        // FRect fontRect = { corner, fontSize };
        TRY(SDL_SetRenderDrawColor(renderer, color.r(), color.g(), color.b(), 255));
        TRY(SDL_RenderFillRect(renderer, &rect.inner));
        TRY(SDL_SetRenderDrawColor(renderer, oldColor.r(), oldColor.g(), oldColor.b(), oldColor.a()));
        // if (fontTexture != nullptr)
        //     TRY(SDL_RenderTexture(renderer, fontTexture, nullptr, &fontRect.inner));
        fontTexture.drawAt(renderer, corner);
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
            rows * Cell::cellSize().h(),
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
    FRect Row::getTitleRect() const
    {
        return {corner, Cell::cellSize()};
    }
    void Row::addChild(Cell* child, int index)
    {
        if (index < 0)
            index = children.size() + index + 1;
        SDL_assert(index >= 0 && index <= children.size());
        children.insert(children.begin() + index, child);
        arrangeChildren();
    }
    int Row::dropIndex(FPoint point) const
    {
        const auto cellSize = Cell::cellSize();
        FPoint rel = { point.x() - corner.x() - cellSize.w(), point.y() - corner.y() };
        int index = static_cast<int>(rel.y() / cellSize.h()) * columns + std::min(static_cast<int>(rel.x() / cellSize.w() + 0.5), columns);
        SDL_assert(index >= 0);
        if (index > children.size())
            index = children.size();
        return index;
    }
    void Row::resizeCells(FSize size)
    {
        for (auto cell : children)
        {
            cell->setSize(size);
        }
        //redundant? should be cheap though
        arrangeChildren();
    }
    int Row::removeChild(Cell* child)
    {
        auto it = std::find(children.begin(), children.end(), child);
        if (it == children.end())
            return -1;
        const auto index = it - children.begin();
        children.erase(it);
        arrangeChildren();
        return index;
    }
    //use a flow algorithm
    void Row::arrangeChildren()
    {
        // std::cout << "Arrange Children" << std::endl;
        int width = Window::getWindowSize().w();
        const auto cellSize = Cell::cellSize();
        rows = 1;
        columns = 0;
        int row = 0;
        int col = 1;
        for (auto child : children)
        {
            child->moveTo({cellSize.w() * col + corner.x(), cellSize.h() * row + corner.y()});
            if (cellSize.w() * (col+2) <= width)
            {
                col++;
                columns = std::max(columns, col);
                rows = std::max(rows, row+1);
            }
            else
            {
                col = 1;
                row++;
                rows = std::max(row, rows);
            }
        }
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
    std::unordered_set<uint32_t> Row::rowIds;
    uint32_t Row::getId() const
    {
        return id;
    }
    uint32_t Row::getNewId()
    {
        for (uint32_t i = 1; i < std::numeric_limits<uint32_t>::max(); i++)
        {
            if (!rowIds.contains(i))
            {
                rowIds.insert(i);
                return i;
            }
        }
        static const char* errMsg = "Could not create row id";
        std::cerr << errMsg << std::endl;
        throw(errMsg);
    }
}
