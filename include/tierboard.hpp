#ifndef H_6BE0A9E38751468989913A8C2287285A
#define H_6BE0A9E38751468989913A8C2287285A

#include "utils.hpp"
#include "tierrow.hpp"
#include "graphics.hpp"
#include <vector>


namespace Tier
{
    class Board : public Graphics::Drawable
    {
        public:
            void draw(SDL_Renderer* renderer) const override;
            FSize getSize() const override;
            FPoint getCorner() const override;
            void setCorner(FPoint point) override;
            void addChild(Row* child, int index=-1);

            void arrangeChildren();
            void resizeCells(FSize size);
            std::vector<Row*> children;
        private:
            
            FRect rect;
    };
}

#endif //H_6BE0A9E38751468989913A8C2287285A