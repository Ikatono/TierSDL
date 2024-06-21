#include "window.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstring>

namespace Window
{
    const char* getBestRenderer()
    {
        const auto numRen = SDL_GetNumRenderDrivers();
        for (auto priority : rendererPriority)
        {
            for (auto i = 0; i < numRen; i++)
            {
                const auto renderer = SDL_GetRenderDriver(i);
                if (!std::strcmp(renderer, priority))
                    return priority;
            }
        }
        return nullptr;
    }
    static SDL_Window* window = nullptr;
    static SDL_Renderer* renderer = nullptr;
    static Size windowSize = { 800, 600 };
    SDL_Window* createWindow()
    {
        const auto size = getWindowSize();
        window = SDL_CreateWindow("Tier SDL", size.w(), size.h(), 0);
        auto* const rendererName = getBestRenderer();
        std::cout << "Rendering engine: " << rendererName << std::endl;
        renderer = SDL_CreateRenderer(window, rendererName);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        return window;
    }
    void destroyWindow()
    {
        if (renderer)
            SDL_DestroyRenderer(renderer);
        if (window)
            SDL_DestroyWindow(window);
    }
    SDL_Window* getWindow()
    {
        return window;
    }
    SDL_Renderer* getRenderer()
    {
        return renderer;
    }
    void setWindowSize(Size size)
    {
        windowSize = size;
    }
    Size getWindowSize()
    {
        return windowSize;
    }
    void fillWindow(Color color)
    {
        Color oldColor;
        SDL_GetRenderDrawColor(renderer, &oldColor.r(), &oldColor.g(), &oldColor.b(), &oldColor.a());
        SDL_SetRenderDrawColor(renderer, color.r(), color.g(), color.b(), color.a());
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, oldColor.r(), oldColor.g(), oldColor.b(), oldColor.a());
    }
    int renderWindow()
    {
        return SDL_RenderPresent(renderer);
    }
}
