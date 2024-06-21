#ifndef H_8DC539D4042B45C7BA03F712BC8BA382
#define H_8DC539D4042B45C7BA03F712BC8BA382

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_pixels.h>
#include <algorithm>
#include <limits>

struct FSize
{
    FSize() : _w(0), _h(0) { }
    FSize(float width, float height) : _w(width), _h(height) { }
    float& w() { return _w; }
    const float& w() const { return _w; }
    float& h() { return _h; }
    const float& h() const { return _h; }
    FSize merge(const FSize& other) const { return { std::max(_w, other._w), std::max(_h, other._h) }; }
    private:
        float _w, _h;
};

struct Size
{
    Size() : _w(0), _h(0) { }
    Size(int width, int height) : _w(width), _h(height) { }
    int& w() { return _w; }
    const int& w() const { return _w; }
    int& h() { return _h; }
    const int& h() const { return _h; }
    Size merge(const Size& other) const { return { std::max(_w, other._w), std::max(_h, other._h) }; }
    FSize toFloat() const { return { static_cast<float>(_w), static_cast<float>(_h) }; }
    private:
        int _w, _h;
};

struct FPoint
{
    SDL_FPoint inner;
    FPoint(float x, float y)
        : inner({x, y})
    { }
    FPoint(SDL_FPoint point)
        : inner(point)
    { }
    FPoint()
        : inner()
    { }
    operator SDL_FPoint() const { return inner; }
    float& x() { return inner.x; }
    const float& x() const { return inner.x; }
    float& y() { return inner.y; }
    const float& y() const { return inner.y; }
};

struct Point
{
    SDL_Point inner;
    Point(int x, int y)
        : inner({x, y})
    { }
    Point(SDL_Point point)
        : inner(point)
    { }
    Point()
        : inner()
    { }
    operator SDL_Point() const { return inner; }
    FPoint toFloat() const { return { static_cast<float>(inner.x), static_cast<float>(inner.y) }; }
    int& x() { return inner.x; }
    const int& x() const { return inner.x; }
    int& y() { return inner.y; }
    const int& y() const { return inner.y; }
};

struct FRect
{
    SDL_FRect inner;
    FRect(float x, float y, float w, float h)
        : inner({x, y, w, h})
    { }
    FRect(FPoint point, FSize size)
        : inner({point.x(), point.y(), size.w(), size.h()})
    { }
    FRect()
        : inner()
    { }
    operator SDL_FRect() const { return inner; }
    float& x() { return inner.x; }
    const float& x() const { return inner.x; }
    float& y() { return inner.y; }
    const float& y() const { return inner.y; }
    float& w() { return inner.w; }
    const float& w() const { return inner.w; }
    float& h() { return inner.h; }
    const float& h() const { return inner.h; }
    FSize size() const { return {inner.w, inner.h}; }
    FPoint corner() const { return {inner.x, inner.y}; }
    FPoint center() const { return {inner.x + inner.w/2, inner.y + inner.h/2}; }
    bool contains(Point point) const
    {
        return !(point.x() < x() || point.x() > x() + w() || point.y() < y() || point.y() > y() + w());
    }
    bool contains(FPoint point)  const
    {
        return !(point.x() < x() || point.x() > x() + w() || point.y() < y() || point.y() > y() + w());
    }
};

struct Rect
{
    SDL_Rect inner;
    Rect(int x, int y, int w, int h)
        : inner({x, y, w, h})
    { }
    Rect(Point point, Size size)
        : inner({point.x(), point.y(), size.w(), size.h()})
    { }
    Rect()
        : inner()
    { }
    FRect toFloat() const { return { static_cast<float>(inner.x), static_cast<float>(inner.y), static_cast<float>(inner.w), static_cast<float>(inner.h) }; }
    int& x() { return inner.x; }
    const int& x() const { return inner.x; }
    int& y() { return inner.y; }
    const int& y() const { return inner.y; }
    int& w() { return inner.w; }
    const int& w() const { return inner.w; }
    int& h() { return inner.h; }
    const int& h() const { return inner.h; }
    Size size() const { return {inner.w, inner.h}; }
    Point corner() const { return {inner.x, inner.y}; }
    Point center() const { return {inner.x + inner.w/2, inner.y + inner.h/2}; }
    bool contains(Point point) const
    {
        return !(point.x() < x() || point.x() > x() + w() || point.y() < y() || point.y() > y() + w());
    }
    bool contains(FPoint point) const
    {
        return !(point.x() < x() || point.x() > x() + w() || point.y() < y() || point.y() > y() + w());
    }
};

struct Color
{
    SDL_Color inner;
    Color(SDL_Color color) : inner(inner)
    { }
    Color() : inner({0, 0, 0, std::numeric_limits<Uint8>::max()})
    { }
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a=std::numeric_limits<Uint8>::max())
        : inner({r, g, b, a})
    { }
    operator SDL_Color() const { return inner; }
    Uint8& r() { return inner.r; }
    const Uint8& r() const { return inner.r; }
    Uint8& g() { return inner.g; }
    const Uint8& g() const { return inner.g; }
    Uint8& b() { return inner.b; }
    const Uint8& b() const { return inner.b; }
    Uint8& a() { return inner.a; }
    const Uint8& a() const { return inner.a; }
};

#define WHITE Color(255, 255, 255, 255)
#define BLACK Color(0, 0, 0, 255)
#define RED   Color(255, 0, 0, 255)
#define GREEN Color(0, 255, 0, 255)
#define BLUE  Color(0, 0, 255, 255)

#endif //H_8DC539D4042B45C7BA03F712BC8BA382