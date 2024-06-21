#include "mouse.hpp"

namespace Mouse
{
    FPoint getPosition()
    {
        FPoint point;
        SDL_GetMouseState(&point.x(), &point.y());
        return point;
    }
    Buttons getButtons()
    {
        FPoint point;
        const auto buttons = SDL_GetMouseState(&point.x(), &point.y());
        return {
            .left   = (buttons & SDL_BUTTON_LMASK)  != 0,
            .right  = (buttons & SDL_BUTTON_RMASK)  != 0,
            .middle = (buttons & SDL_BUTTON_MMASK)  != 0,
            .x1     = (buttons & SDL_BUTTON_X1MASK) != 0,
            .x2     = (buttons & SDL_BUTTON_X2MASK) != 0,
        };
    }
}
