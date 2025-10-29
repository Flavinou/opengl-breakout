#include "PowerUp.h"

#include <algorithm>

#include "Application.h"
#include "ResourceManager.h"
#include "Physics/Physics2D.h"

PowerUp::PowerUp(const Type type, const float duration, const glm::vec4& color, const glm::vec2& position,
    const std::shared_ptr<Texture2D>& texture, EventFn&& modifierCallback, EventFn&& undoCallback)
    : GameObject(position, POWER_UP_DEFAULT_SIZE, texture, 0.0f, color, POWER_UP_DEFAULT_VELOCITY)
    , m_Type(type)
    , m_ModifierCallback(std::move(modifierCallback))
    , m_UndoCallback(std::move(undoCallback))
    , m_Duration(duration)
{
}

void PowerUp::Enable()
{
    assert(!m_Enabled);

    m_Destroyed = true;
    m_Enabled = true;

    if (m_ModifierHandled)
        return;

    m_ModifierHandled |= m_ModifierCallback(Application::Get().GetGame());
}

void PowerUp::Disable()
{
    assert(m_Enabled);

    m_Enabled = false;

    if (m_UndoHandled)
        return;

    m_UndoHandled |= m_UndoCallback(Application::Get().GetGame());
}

void PowerUp::Update(float deltaTime)
{
    m_Position += m_Velocity * deltaTime;

    if (!m_Enabled)
        return;

    m_Duration -= deltaTime;

    if (m_Duration <= 0.0f)
        Disable();
}

void PowerUp::DoCollision(const GameObject& other)
{
    if (m_Destroyed)
        return;

    const auto& game = Application::Get().GetGame();
    if (m_Position.y >= game.GetViewportHeight())
    {
        m_Destroyed = true;
    }

    CollisionResult result = Physics2D::CheckCollisionAABB(other, *this);
    if (!std::get<0>(result)) return; // No collision, return early

    // A collision occured with the player, trigger the power-up effect !
    Enable();
}

void PowerUpEmitter::Update(float deltaTime)
{
    for (PowerUp& powerUp : m_PowerUps)
    {
        powerUp.Update(deltaTime);
    }

    m_PowerUps.erase(
        std::remove_if(
        m_PowerUps.begin(), 
        m_PowerUps.end(),
        [](const PowerUp& powerUp)
        {
            return powerUp.IsDestroyed() && !powerUp.IsEnabled();
        }), 
        m_PowerUps.end()
    );
}

void PowerUpEmitter::Render() const
{
    for (const PowerUp& powerUp : m_PowerUps)
    {
        if (powerUp.IsDestroyed())
            continue;

        const auto& game = Application::Get().GetGame();
        powerUp.Draw(game.GetRenderer());
    }
}

void PowerUpEmitter::DoCollisions(const GameObject& other)
{
    for (PowerUp& powerUp : m_PowerUps)
    {
        powerUp.DoCollision(other);
    }
}

bool PowerUpEmitter::ShouldSpawn(const unsigned int chance)
{
    const unsigned int random = rand() % chance;
    return random == 0;
}

void PowerUpEmitter::SpawnPowerUps(const GameObject& block)
{
    if (ShouldSpawn(75)) // 1 in 75 chance
        m_PowerUps.emplace_back(
            PowerUp::Speed, 0.0f,
            glm::vec4(0.5f, 0.5f, 1.0f, 1.0f),
            block.GetPosition(),
            ResourceManager::Instance().GetTexture("speed"),
            [](auto& game)
            {
                auto& ball = game.GetBall();
                ball.SetVelocity(ball.GetVelocity() * 1.2f);
                return true;
            },
            [this](auto&){ return true; }
        );
    if (ShouldSpawn(75))
        m_PowerUps.emplace_back(
            PowerUp::Sticky, 20.0f,
            glm::vec4(1.0f, 0.5f, 1.0f, 1.0f),
            block.GetPosition(),
            ResourceManager::Instance().GetTexture("sticky"),
            [](auto& game)
            {
                auto& ball = game.GetBall();
                auto& player = game.GetPlayer();
                ball.SetSticky(true);
                player.SetColor(glm::vec4(1.0f, 0.5f, 1.0f, 1.0f));
                return true;
            },
            [this](auto& game)
            {
                if (IsOtherPowerUpActive(PowerUp::Sticky))
                    return true;

                auto& ball = game.GetBall();
                auto& player = game.GetPlayer();
                ball.SetSticky(false);
                player.SetColor(glm::vec4(1.0f));
                return true;
            }
        );
    if (ShouldSpawn(75))
        m_PowerUps.emplace_back(
            PowerUp::PassThrough, 10.0f,
            glm::vec4(0.5f, 1.0f, 0.5f, 1.0f),
            block.GetPosition(),
            ResourceManager::Instance().GetTexture("passthrough"),
            [] (auto& game)
            {
                auto& ball = game.GetBall();
                ball.SetPassThrough(true);
                ball.SetColor(glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));
                return true;
            },
            [this](auto& game)
            {
                if (IsOtherPowerUpActive(PowerUp::PassThrough))
                    return true;

                auto& ball = game.GetBall();
                ball.SetPassThrough(false);
                ball.SetColor(glm::vec4(1.0f));
                return true;
            }
        );
    if (ShouldSpawn(5))
        m_PowerUps.emplace_back(
            PowerUp::PadSizeIncrease, 0.0f,
            glm::vec4(1.0f, 0.6f, 0.4f, 1.0f),
            block.GetPosition(),
            ResourceManager::Instance().GetTexture("padsizeincrease"),
            [](auto& game)
            {
                auto& player = game.GetPlayer();
                const auto& size = player.GetSize();
                player.SetSize(glm::vec2(size.x + 50, size.y));
                return true;
            },
            [this](auto&){return true;}
        );
    if (ShouldSpawn(15)) // negative powerups should spawn more often
        m_PowerUps.emplace_back(
            PowerUp::Confuse, 15.0f,
            glm::vec4(1.0f, 0.3f, 0.3f, 1.0f),
            block.GetPosition(),
            ResourceManager::Instance().GetTexture("confuse"),
            [](auto& game)
            {
                auto& postProcess = game.GetPostProcess();
                if (postProcess.GetMode() == PostProcess::Chaos)
                    return true;

                postProcess.SetMode(PostProcess::Confuse);
                return true;
            },
            [this](auto& game)
            {
                if (IsOtherPowerUpActive(PowerUp::Confuse))
                    return false;

                auto& postProcess = game.GetPostProcess();
                postProcess.SetMode(PostProcess::None);
                return true;
            }
        );
    if (ShouldSpawn(5))
        m_PowerUps.emplace_back(
            PowerUp::Chaos, 15.0f,
            glm::vec4(0.9f, 0.25f, 0.25f, 1.0f),
            block.GetPosition(),
            ResourceManager::Instance().GetTexture("chaos"),
            [](auto& game)
            {
                auto& postProcess = game.GetPostProcess();
                if (postProcess.GetMode() == PostProcess::Confuse)
                    return true;

                postProcess.SetMode(PostProcess::Chaos);
                return true;
            },
            [this](auto& game)
            {
                if (IsOtherPowerUpActive(PowerUp::Chaos))
                    return true;

                auto& postProcess = game.GetPostProcess();
                postProcess.SetMode(PostProcess::None);
                return true;
            }
        );
}

bool PowerUpEmitter::IsOtherPowerUpActive(PowerUp::Type type) const
{
    // Check if another PowerUp of the same type is still active
    // in which case we don't disable its effect (yet)
    return std::any_of(m_PowerUps.cbegin(), m_PowerUps.cend(), 
        [type](const PowerUp& powerUp)
        {
            return powerUp.IsEnabled() && powerUp.GetType() == type;
        });
}
