#include <abyssutils/math/Box.hpp>
#include <limits>
#include <algorithm>
#include <cmath>

#define VECINRECT(vec, x1, y1, x2, y2) ((vec).x >= (x1) && (vec).x < (x2) && (vec).y >= (y1) && (vec).y < (y2))

using namespace Abyssutils::Math;

constexpr double HALF    = 0.5;
constexpr double DOUBLE  = 2.0;
constexpr double EPSILON = 1e-9;

CBox&            Abyssutils::Math::CBox::scale(double scale) {
    x *= scale;
    y *= scale;
    w *= scale;
    h *= scale;

    return *this;
}

CBox& Abyssutils::Math::CBox::scale(const Vector2D& scale) {
    x *= scale.x;
    y *= scale.y;
    w *= scale.x;
    h *= scale.y;

    return *this;
}

CBox& Abyssutils::Math::CBox::translate(const Vector2D& vec) {
    x += vec.x;
    y += vec.y;

    return *this;
}

Vector2D Abyssutils::Math::CBox::middle() const {
    return Vector2D{x + (w * HALF), y + (h * HALF)};
}

bool Abyssutils::Math::CBox::containsPoint(const Vector2D& vec) const {
    return VECINRECT(vec, x, y, x + w, y + h);
}

bool Abyssutils::Math::CBox::empty() const {
    return std::fabs(w) < EPSILON || std::fabs(h) < EPSILON;
}

CBox& Abyssutils::Math::CBox::round() {
    double roundedX = std::round(x);
    double roundedY = std::round(y);
    double newW     = x + w - roundedX;
    double newH     = y + h - roundedY;

    x = roundedX;
    y = roundedY;
    w = std::round(newW);
    h = std::round(newH);

    return *this;
}

CBox& Abyssutils::Math::CBox::transform(const eTransform t, double w, double h) {

    CBox temp = *this;

    if (t % 2 == 0) {
        width  = temp.width;
        height = temp.height;
    } else {
        width  = temp.height;
        height = temp.width;
    }

    switch (t) {
        case ABYSSUTILS_TRANSFORM_NORMAL:
            x = temp.x;
            y = temp.y;
            break;
        case ABYSSUTILS_TRANSFORM_90:
            x = h - temp.y - temp.height;
            y = temp.x;
            break;
        case ABYSSUTILS_TRANSFORM_180:
            x = w - temp.x - temp.width;
            y = h - temp.y - temp.height;
            break;
        case ABYSSUTILS_TRANSFORM_270:
            x = temp.y;
            y = w - temp.x - temp.width;
            break;
        case ABYSSUTILS_TRANSFORM_FLIPPED:
            x = w - temp.x - temp.width;
            y = temp.y;
            break;
        case ABYSSUTILS_TRANSFORM_FLIPPED_90:
            x = temp.y;
            y = temp.x;
            break;
        case ABYSSUTILS_TRANSFORM_FLIPPED_180:
            x = temp.x;
            y = h - temp.y - temp.height;
            break;
        case ABYSSUTILS_TRANSFORM_FLIPPED_270:
            x = h - temp.y - temp.height;
            y = w - temp.x - temp.width;
            break;
    }

    return *this;
}

CBox& Abyssutils::Math::CBox::addExtents(const SBoxExtents& e) {
    x -= e.topLeft.x;
    y -= e.topLeft.y;
    w += e.topLeft.x + e.bottomRight.x;
    h += e.topLeft.y + e.bottomRight.y;

    return *this;
}

CBox& Abyssutils::Math::CBox::scaleFromCenter(double scale) {
    double oldW = w, oldH = h;

    w *= scale;
    h *= scale;

    x -= (w - oldW) * HALF;
    y -= (h - oldH) * HALF;

    return *this;
}

CBox& Abyssutils::Math::CBox::expand(const double& value) {
    x -= value;
    y -= value;
    w += value * DOUBLE;
    h += value * DOUBLE;

    if (w <= EPSILON || h <= EPSILON) {
        w = 0;
        h = 0;
    }

    return *this;
}

CBox& Abyssutils::Math::CBox::noNegativeSize() {
    w = std::clamp(w, 0.0, std::numeric_limits<double>::infinity());
    h = std::clamp(h, 0.0, std::numeric_limits<double>::infinity());

    return *this;
}

CBox Abyssutils::Math::CBox::intersection(const CBox& other) const {
    const double newX      = std::max(x, other.x);
    const double newY      = std::max(y, other.y);
    const double newBottom = std::min(y + h, other.y + other.h);
    const double newRight  = std::min(x + w, other.x + other.w);
    double       newW      = newRight - newX;
    double       newH      = newBottom - newY;

    if (newW <= EPSILON || newH <= EPSILON) {
        newW = 0;
        newH = 0;
    }

    return {newX, newY, newW, newH};
}

bool Abyssutils::Math::CBox::overlaps(const CBox& other) const {
    return (other.x + other.w > x) && (x + w > other.x) && (other.y + other.h > y) && (y + h > other.y);
}

bool Abyssutils::Math::CBox::inside(const CBox& bound) const {
    return bound.x < x && bound.y < y && x + w < bound.x + bound.w && y + h < bound.y + bound.h;
}

CBox Abyssutils::Math::CBox::roundInternal() {
    double flooredX = std::floor(x);
    double flooredY = std::floor(y);
    double newW     = x + w - flooredX;
    double newH     = y + h - flooredY;

    return CBox{flooredX, flooredY, std::floor(newW), std::floor(newH)};
}

CBox Abyssutils::Math::CBox::copy() const {
    return CBox{*this};
}

Vector2D Abyssutils::Math::CBox::pos() const {
    return {x, y};
}

Vector2D Abyssutils::Math::CBox::size() const {
    return {w, h};
}

Vector2D Abyssutils::Math::CBox::extent() const {
    return pos() + size();
}

Vector2D Abyssutils::Math::CBox::closestPoint(const Vector2D& vec) const {
    if (containsPoint(vec))
        return vec;

    Vector2D nv       = vec;
    Vector2D maxPoint = {x + w - EPSILON, y + h - EPSILON};

    if (x < maxPoint.x)
        nv.x = std::clamp(nv.x, x, maxPoint.x);
    else
        nv.x = x;
    if (y < maxPoint.y)
        nv.y = std::clamp(nv.y, y, maxPoint.y);
    else
        nv.y = y;

    if (std::fabs(nv.x - x) < EPSILON)
        nv.x = x;
    else if (std::fabs(nv.x - (maxPoint.x)) < EPSILON)
        nv.x = maxPoint.x;

    if (std::fabs(nv.y - y) < EPSILON)
        nv.y = y;
    else if (std::fabs(nv.y - (maxPoint.y)) < EPSILON)
        nv.y = maxPoint.y;

    return nv;
}

SBoxExtents Abyssutils::Math::CBox::extentsFrom(const CBox& small) {
    return {.topLeft = {small.x - x, small.y - y}, .bottomRight = {w - small.w - (small.x - x), h - small.h - (small.y - y)}};
}
