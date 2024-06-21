#ifndef H_A51F75F8EE604EEF999F934EEB53AC0B
#define H_A51F75F8EE604EEF999F934EEB53AC0B

#include "tiercell.hpp"
#include "text.hpp"
#include "utils.hpp"
#include "graphics.hpp"
#include <vector>
#include <string>

namespace Tier
{
    class Row;
    class TitleCell : public Graphics::Drawable
    {
        
    };
    class Row : public Graphics::Drawable
    {
        public:
            Row();
            const std::string* getText() const;
            void setText(std::string text);
            Color getBackgroundColor() const;
            void setBackgroundColor(Color color);
            Color getTextColor() const;
            void setTextColor(Color color);
            int getTextSize() const;
            void setTextSize(int ptsize);
            void draw(SDL_Renderer* renderer) const override;
            FRect getRect() const;
            FPoint getCorner() const override;
            FSize getSize() const override;
            void setCorner(FPoint point) override;
            float getNeededHeight() const;
            void addChild(Cell* child, int index=-1);
            void removeChild(Cell* child);
            void resizeCells(FSize size);
            //should be private, refactor
            std::vector<Cell*> children;
            void arrangeChildren();
            bool contains(Point point) const;
            bool contains(FPoint point) const;
        private:
            
            Color backgroundColor;
            std::string text;
            Color textColor;
            int textSize;
            FPoint corner;
            float neededHeight;
            void updateFontTexture();
    };
}

#endif //H_A51F75F8EE604EEF999F934EEB53AC0B
