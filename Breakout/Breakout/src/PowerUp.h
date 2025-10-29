#pragma once

#include <functional>
#include <vector>

#include "BallObject.h"
#include "GameObject.h"

#include <glm/glm.hpp>

class Game;

static constexpr glm::vec2 POWER_UP_DEFAULT_SIZE(60.0f, 20.0f);
static constexpr glm::vec2 POWER_UP_DEFAULT_VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
    using EventFn = std::function<bool(Game&)>;

    enum Type : std::uint8_t
    {
        None = 0,
        Speed,
        Sticky,
        PassThrough,
        PadSizeIncrease,
        Confuse,
        Chaos
    };

    PowerUp(Type type, float duration, const glm::vec4& color, const glm::vec2& position,
            const std::shared_ptr<Texture2D>& texture, 
            EventFn&& modifierCallback,
            EventFn&& undoCallback);

    Type GetType() const { return m_Type; }
    float GetDuration() const { return m_Duration; }
    bool IsEnabled() const { return m_Enabled; }

    void SetDuration(float value) { m_Duration = value; }

    void Update(float deltaTime);
    void DoCollision(const GameObject& other);
private:
    void Enable();
    void Disable();
private:
    Type m_Type;
    EventFn m_ModifierCallback;
    EventFn m_UndoCallback;
    float m_Duration{ 0.0f };
    bool m_Enabled{ false };
    bool m_ModifierHandled{ false };
    bool m_UndoHandled{ false };
};

class PowerUpEmitter
{
public:
    PowerUpEmitter() = default;
    ~PowerUpEmitter() = default;

    void Update(float deltaTime);
    void Render() const;
    void DoCollisions(const GameObject& other);

    static bool ShouldSpawn(unsigned int chance);
    void SpawnPowerUps(const GameObject& block);
    bool IsOtherPowerUpActive(PowerUp::Type type) const;
private:
    std::vector<PowerUp> m_PowerUps{};
};