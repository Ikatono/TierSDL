#ifndef H_C46264AE64B3401BBC99882FA18A800E
#define H_C46264AE64B3401BBC99882FA18A800E

#include "utils.hpp"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <array>
#include <string>

namespace Window
{
    constexpr std::array rendererPriority {
        "vulkan",
        "opengl",
        "opengles2",
        "software",
    };
    // constexpr std::array rendererPriority {
    //     "opengl",
    //     "vulkan",
    //     "opengles2",
    //     "software",
    // };
    const char* getBestRenderer();
    SDL_Window* createWindow(Size size);
    void destroyWindow();
    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();
    void setWindowSize(Size size);
    Size getWindowSize();
    void fillWindow(Color color = WHITE);
    int renderWindow();
}

#endif //H_C46264AE64B3401BBC99882FA18A800E