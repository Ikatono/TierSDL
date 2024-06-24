#ifndef H_BF9C848BF1E54B10BFA82F7B36A860FF
#define H_BF9C848BF1E54B10BFA82F7B36A860FF

#include "graphics.hpp"
#include "utils.hpp"
#include "SDL3_image/SDL_image.h"
#include <vector>

namespace Graphics
{
    struct ImageType
    {
        uint32_t value;
        static const ImageType Jpeg;
        static const ImageType Png;
        static const ImageType Tif;
        static const ImageType Webp;
        static const ImageType Jxl;
        static const ImageType Avif;
        bool hasFlags(ImageType other) const;
        void addFlags(ImageType other);
        void removeFlags(ImageType other);
    };

    //initializes every known image type one at a time, and returns the ones that succeeded
    ImageType imageInit();
    Drawing loadImage(const char* filepath);
    std::vector<const char*> getImageTypeNames(ImageType types);
}

#endif //H_BF9C848BF1E54B10BFA82F7B36A860FF