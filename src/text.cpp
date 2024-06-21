#include "text.hpp"
#include "window.hpp"
#include <SDL3/SDL_assert.h>
#include <iostream>
#include <unordered_map>

namespace Text
{
    static const char* filepath;
    int init(const char* filepath)
    {
        (void)SDL_GetError();
        if (TTF_Init())
            throw(SDL_GetError());
        Text::filepath = filepath;
        return 0;
    }

    static std::unordered_map<int, std::weak_ptr<TTF_Font>> fonts;

    std::shared_ptr<TTF_Font> getFont(int ptsize)
    {
        SDL_assert(filepath != nullptr);
        auto iter = fonts.find(ptsize);
        if (iter == fonts.end())
        {
            auto font = TTF_OpenFont(filepath, ptsize);
            if (font == nullptr)
                throw(SDL_GetError());
            std::shared_ptr<TTF_Font> ptr(font,[](TTF_Font* f) { TTF_CloseFont(f); });
            fonts[ptsize] = std::weak_ptr<TTF_Font>(ptr);
            return ptr;
        }
        else
        {
            if (auto sptr = iter->second.lock())
            {
                return sptr;
            }
            else
            {
                auto font = TTF_OpenFont(filepath, ptsize);
                if (font == nullptr)
                    throw(SDL_GetError());
                std::shared_ptr<TTF_Font> ptr(font,[](TTF_Font* f) { TTF_CloseFont(f); });
                fonts[ptsize] = std::weak_ptr<TTF_Font>(ptr);
                return ptr;
            }
        }
    }
    SDL_Texture* renderText(const char* text, TTF_Font* font, Color color, FRect rect, SDL_Renderer* renderer)
    {
        
        if (!renderer)
            renderer = Window::getRenderer();
        SDL_GetError();
        auto* surface = TTF_RenderUTF8_Blended_Wrapped(font, text,
            color, static_cast<int>(rect.w()));
        if (!surface)
        {
            auto err = SDL_GetError();
            std::cerr << "Font render error: " << err << std::endl;
            throw(err);
        }
        auto* texture = SDL_CreateTextureFromSurface(renderer, surface);
        // SDL_DestroySurface(surface);
        return texture;
    }
}