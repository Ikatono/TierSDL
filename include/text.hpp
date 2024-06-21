#ifndef H_369525E749D945D9A267F110EBB23AFD
#define H_369525E749D945D9A267F110EBB23AFD

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include "utils.hpp"
#include <memory>

namespace Text
{
    // class Font
    // {
    //     public:
    //         Font(int ptsize);
    //         const int ptsize;
    //     private:
    //         std::shared_ptr<TTF_Font> ptr;
    // };
    int init(const char* filepath);
    std::shared_ptr<TTF_Font> getFont(int ptsize);
    // void setFont(const char* filepath, int ptsize);
    // void resizeFont(int ptsize);
    SDL_Texture* renderText(const char* text, TTF_Font* font, Color color, FRect rect, SDL_Renderer* renderer=nullptr);
}

#endif //H_369525E749D945D9A267F110EBB23AFD