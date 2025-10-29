#pragma once

#include "GameObject.h"
#include "Renderer/Texture2D.h"

#include <glm/glm.hpp>
#include <memory>

class BallObject final : public GameObject
{
public:
    using super = GameObject;

    enum State : std::uint8_t
    {
        None = 0,
        Stuck,
        Sticky,
        Passthrough
    };

    BallObject();
    BallObject(const glm::vec2& position, const glm::vec2& velocity, const std::shared_ptr<Texture2D>& sprite, float radius);
    ~BallObject() override = default;

    void SetStuck(bool value) { m_Stuck = value; }
    void SetSticky(bool value) { m_Sticky = value; }
    void SetPassThrough(bool value) { m_PassThrough = value; }

    bool IsStuck() const { return m_Stuck; }
    bool IsSticky() const { return m_Sticky; }
    bool IsPassThrough() const { return m_PassThrough; }

    float GetRadius() const { return m_Radius; }

    glm::vec2 Move(float deltaTime, unsigned int viewportWidth);
    void Reset(const glm::vec2& position, const glm::vec2& velocity) override;

private:
    float m_Radius;
    bool m_Stuck{ true };
    bool m_Sticky{ false };
    bool m_PassThrough{ false };
};
