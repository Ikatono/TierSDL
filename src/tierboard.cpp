#include "tierboard.hpp"

namespace Tier
{
    void Board::draw(SDL_Renderer* renderer) const
    {
        for (auto& row : children)
        {
            row->draw(renderer);
        }
    }
    FSize Board::getSize() const
    {
        return rect.size();
    }
    FPoint Board::getCorner() const
    {
        return rect.corner();
    }
    void Board::setCorner(FPoint point)
    {
        rect.x() = point.x();
        rect.y() = point.y();
    }
    void Board::addChild(Row* child, int index /* =-1 */)
    {
        if (index < 0)
            index = children.size() + index + 1;
        SDL_assert(index >= 0 && index <= children.size());
        children.insert(children.begin() + index, child);
        arrangeChildren();
    }
    void Board::arrangeChildren()
    {
        float height = 0;
        for (auto& row : children)
        {
            row->setCorner({0, height});
            row->arrangeChildren();
            height += row->getSize().h();
        }
    }
    void Board::resizeCells(FSize size)
    {
        
    }
}
