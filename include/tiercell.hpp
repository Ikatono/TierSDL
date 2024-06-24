#ifndef H_737F0697F5694FD08E183F42F412C8A3
#define H_737F0697F5694FD08E183F42F412C8A3

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include "utils.hpp"
#include "graphics.hpp"
#include "text.hpp"
#include <string>
#include <unordered_set>

namespace Tier
{
    class Cell : public Graphics::Drawable
    {
        public:
            Cell();
            ~Cell();
            Color backgroundColor;
            std::string text;
            Color fontColor;
            Color getBackgroundColor() const;
            void setBackgroundColor(Color color);
            Color getFontColor() const;
            void setFontColor(Color color);
            int getFontSize() const;
            void setFontSize(int ptsize);
            void moveBy(float x, float y);
            void moveTo(FPoint point);
            void setText(std::string newText);
            const std::string* peekText() const;
            void draw(SDL_Renderer* renderer) const override;
            bool contains(Point point) const;
            bool contains(FPoint point) const;
            FPoint getCorner() const override;
            FSize getSize() const override;
            FRect getRect() const;
            void setCorner(FPoint point) override;
            static void setSize(FSize size);
            void setImage(const char* filepath);
            static FSize cellSize();
            uint32_t getId() const;
            static std::unordered_set<uint32_t> cellIds;
        private:
            static FSize size;
            FPoint corner;
            Graphics::Drawing fontTexture;
            std::shared_ptr<TTF_Font> font;
            int fontSize;
            Graphics::Drawing image;
            const uint32_t id;
            void updateFont();
            void updateFontTexture();
            static uint32_t getNewId();
    };
}

#endif //H_737F0697F5694FD08E183F42F412C8A3
