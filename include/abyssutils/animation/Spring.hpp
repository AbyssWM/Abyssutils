#pragma once

#include <abyssutils/animation/AnimationManager.hpp>

#include <chrono>

namespace Abyssutils::Animation {
    void advanceSpring(float& value, float& velocity, const SSpringCurve& spring, std::chrono::duration<float> elapsed);
}
