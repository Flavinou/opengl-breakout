#pragma once

#include "GameObject.h"
#include "Renderer/Texture2D.h"

#include <glm/glm.hpp>
#include <memory>

class BallObject : public GameObject
{
public:
    BallObject();
    BallObject(const glm::vec2& position, const glm::vec2& velocity, const std::shared_ptr<Texture2D>& sprite, float radius);
    ~BallObject() override = default;

    inline void SetStuck(bool value) { m_Stuck = value; }
    inline bool IsStuck() { return m_Stuck; }

    glm::vec2 Move(float deltaTime, unsigned int viewportWidth);
    void Reset(const glm::vec2& position, const glm::vec2& velocity);

private:
    float m_Radius;
    bool m_Stuck;
};
