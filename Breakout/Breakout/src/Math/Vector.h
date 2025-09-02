#pragma once

#include <cstdint>
#include <glm/glm.hpp>

enum Direction : std::uint8_t   
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

inline Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] =
    {
        glm::vec2(0.0f, 1.0f),  // up
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f)  // left
    };
    float max = 0.0f;
    unsigned int bestMatch = -1;
    for (unsigned int i = 0; i < 4; ++i)
    {
        float dot = glm::dot(glm::normalize(target), compass[i]);
        if (dot > max)
        {
            max = dot;
            bestMatch = i;
        }
    }

    return static_cast<Direction>(bestMatch);
}