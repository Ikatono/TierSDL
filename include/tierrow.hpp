#ifndef H_A51F75F8EE604EEF999F934EEB53AC0B
#define H_A51F75F8EE604EEF999F934EEB53AC0B

#include "tiercell.hpp"
#include "text.hpp"
#include "utils.hpp"
#include "graphics.hpp"
#include <unordered_set>
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
            ~Row();
            const std::string* getText() const;
            void setText(std::string text);
            Color getBackgroundColor() const;
            void setColor(Color color);
            Color getTextColor() const;
            void setTextColor(Color color);
            int getTextSize() const;
            void setTextSize(int ptsize);
            void draw(SDL_Renderer* renderer) const override;
            FRect getRect() const;
            FPoint getCorner() const override;
            FSize getSize() const override;
            void setCorner(FPoint point) override;
            FRect getTitleRect() const;
            void addChild(Cell* child, int index=-1);
            int dropIndex(FPoint point) const;
            //returns the index of the child removed, or -1 if not found
            int removeChild(Cell* child);
            void resizeCells(FSize size);
            //should be private, refactor
            std::vector<Cell*> children;
            void arrangeChildren();
            uint32_t getId() const;
            bool contains(Point point) const;
            bool contains(FPoint point) const;
        
        private:
            Color color;
            std::string text;
            Color textColor;
            int textSize;
            std::shared_ptr<TTF_Font> font;
            //make this a Drawing?
            Graphics::Drawing fontTexture;
            FPoint corner;
            int rows;
            int columns;
            const uint32_t id;
            void updateFontObject();
            void updateFontTexture();
            static uint32_t getNewId();
            static std::unordered_set<uint32_t> rowIds;
    };
}

#endif //H_A51F75F8EE604EEF999F934EEB53AC0B
