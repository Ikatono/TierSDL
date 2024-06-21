#ifndef H_7667C8B4DC514D2F979CCC837E3D48A4
#define H_7667C8B4DC514D2F979CCC837E3D48A4

#include <SDL3/SDL_render.h>
#include "utils.hpp"

namespace Graphics
{

    //simple wrapper around a texture
    //takes ownership of the texture
    class Drawing
    {
        public:
            Drawing();
            Drawing(SDL_Texture* texture);
            Drawing(const Drawing& other) = delete;
            Drawing(Drawing&& other);
            Drawing& operator=(Drawing&& other);
            ~Drawing();
            FSize getSize() const;
            //TODO add scaling option?
            void drawAt(SDL_Renderer* renderer, FPoint point);
        private:
            SDL_Texture* texture;
            // FSize size;
    };
    class Drawable
    {
        public:
            virtual void draw(SDL_Renderer* renderer) const = 0;
            //ignores `hidden` (but not for children)
            //routing this through `draw` is easy but causes some design issues
            Drawing toDrawing(SDL_Renderer* renderer);
            void setHidden(bool hidden);
            bool getHidden() const;
            virtual FSize getSize() const = 0;
            virtual FPoint getCorner() const = 0;
            virtual void setCorner(FPoint point) = 0;
        protected:
            bool hidden = false;
    };
};

#endif //H_7667C8B4DC514D2F979CCC837E3D48A4