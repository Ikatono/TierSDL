#include "image.hpp"
#include "window.hpp"
// #include <array>

namespace Graphics
{
    const ImageType ImageType::Jpeg = {1<<0};
    const ImageType ImageType::Png  = {1<<1};
    const ImageType ImageType::Tif  = {1<<2};
    const ImageType ImageType::Webp = {1<<3};
    const ImageType ImageType::Jxl  = {1<<4};
    const ImageType ImageType::Avif = {1<<5};
    bool ImageType::hasFlags(ImageType other) const
    {
        return !(other.value & ~value);
    }
    void ImageType::addFlags(ImageType other)
    {
        value |= other.value;
    }
    void ImageType::removeFlags(ImageType other)
    {
        value &= ~other.value;
    }
    ImageType imageInit()
    {
        ImageType types;
        const auto ret = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF
            | IMG_INIT_WEBP | IMG_INIT_JXL | IMG_INIT_AVIF);
        if (ret & IMG_INIT_JPG)
            types.addFlags(ImageType::Jpeg);
        if (ret & IMG_INIT_PNG)
            types.addFlags(ImageType::Png);
        if (ret & IMG_INIT_TIF)
            types.addFlags(ImageType::Tif);
        if (ret & IMG_INIT_WEBP)
            types.addFlags(ImageType::Webp);
        if (ret & IMG_INIT_JXL)
            types.addFlags(ImageType::Jxl);
        if (ret & IMG_INIT_AVIF)
            types.addFlags(ImageType::Avif);
        return types;
    }
    std::vector<const char*> getImageTypeNames(ImageType types)
    {
        std::vector<const char*> names;
        if (types.hasFlags(ImageType::Jpeg))
            names.emplace_back("JPEG");
        if (types.hasFlags(ImageType::Png))
            names.emplace_back("PNG");
        if (types.hasFlags(ImageType::Tif))
            names.emplace_back("TIF");
        if (types.hasFlags(ImageType::Webp))
            names.emplace_back("WEBP");
        if (types.hasFlags(ImageType::Jxl))
            names.emplace_back("JXL");
        if (types.hasFlags(ImageType::Avif))
            names.emplace_back("AVIF");
        return names;
    }
    Drawing loadImage(const char* filepath)
    {
        auto* renderer = Window::getRenderer();
        auto* texture = IMG_LoadTexture(renderer, filepath);
        TRY(texture == nullptr);
        //this just seems to make non-alpha images entirely transparent
        // TRY(SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND));
        Drawing drawing(texture);
        return drawing;
    }
}