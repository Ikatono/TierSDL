#ifndef H_369525E749D945D9A267F110EBB23AFD
#define H_369525E749D945D9A267F110EBB23AFD

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include "utils.hpp"
#include "graphics.hpp"
#include <memory>

namespace Text
{
    int init(const char* filepath);
    std::shared_ptr<TTF_Font> getFont(int ptsize);
    //Fonts themselves are automatically deallocated when the last cell using them is destroyed,
    //but a weak pointer is left behind. This cleans up the weak pointers that no longer point to
    //anything
    //Returns: number of unloaded pointers
    int clearUnusedFontObjects();
    Graphics::Drawing renderText(const char* text, TTF_Font* font, Color color, FRect rect, SDL_Renderer* renderer=nullptr);
}

#endif //H_369525E749D945D9A267F110EBB23AFD