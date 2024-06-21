#ifndef H_49CE026F20AF432E96BB189371677DD1
#define H_49CE026F20AF432E96BB189371677DD1

#include <SDL3/SDL_mouse.h>
#include "utils.hpp"

namespace Mouse
{
    FPoint getPosition();
    struct Buttons
    {
        bool left;
        bool right;
        bool middle;
        bool x1;
        bool x2;
    };
    Buttons getButtons();
}

#endif //H_49CE026F20AF432E96BB189371677DD1