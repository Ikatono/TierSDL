#ifndef H_737F0697F5694FD08E183F42F412C8A3
#define H_737F0697F5694FD08E183F42F412C8A3

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include "utils.hpp"
#include "graphics.hpp"
#include "text.hpp"
#include <string>

namespace Tier
{
    class Cell : public Graphics::Drawable
    {
        public:
            explicit Cell(FRect rect);
            Cell(FPoint point, FSize size);
            FRect rect;
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
            void setCorner(FPoint point) override;
        private:
            SDL_Texture* fontTexture;
            FSize fontTextureSize;
            std::shared_ptr<TTF_Font> font;
            int fontSize;
            void updateFont();
            void updateFontTexture();
    };
}

#endif //H_737F0697F5694FD08E183F42F412C8A3
