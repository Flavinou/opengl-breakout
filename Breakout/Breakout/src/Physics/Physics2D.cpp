#include "Physics2D.h"

#include <glm/glm.hpp>

bool Physics2D::CheckCollisionAABB(const GameObject& a, const GameObject& b)
{
    const auto& aPos = a.GetPosition();
    const auto& bPos = b.GetPosition();
    const auto& aSize = a.GetSize();
    const auto& bSize = b.GetSize();

    // Collision along x-axis ?
    bool xCollision = aPos.x + aSize.x >= bPos.x && bPos.x + bSize.x >= aPos.x;

    // Collision along y-axis ?
    bool yCollision = aPos.y + aSize.y >= bPos.y && bPos.y + bSize.y >= aPos.y;

    // Collision ONLY if both axes overlap
    return xCollision && yCollision;
}

bool Physics2D::CheckCollisionCircleAABB(const BallObject& a, const GameObject& b)
{
    const auto& aPos = a.GetPosition();
    const auto& bPos = b.GetPosition();
    const auto& aSize = a.GetSize();
    const auto& bSize = b.GetSize();
    auto radius = a.GetRadius();

    // Get center point of the circle
    glm::vec2 center(aPos + radius);

    // Calculate AABB (center, half-extents)
    glm::vec2 aabb_half_extents(bSize.x / 2.0f, bSize.y / 2.0f);
    glm::vec2 aabb_center(bPos.x + aabb_half_extents.x, bPos.y + aabb_half_extents.y);

    // Get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

    // Add clamped value to AABB center and get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;

    // Get vector between circle center and AABB's closest point
    // If length <= radius, there is a collision
    difference = closest - center;
    return glm::length(difference) < radius;
}
