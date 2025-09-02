#pragma once

#include "Math/Vector.h"
#include "BallObject.h"
#include "GameObject.h"

#include <tuple>

typedef std::tuple<bool, Direction, glm::vec2> CollisionResult;

class Physics2D
{
public:
    static CollisionResult CheckCollisionAABB(const GameObject& a, const GameObject& b);
    static CollisionResult CheckCollisionCircleAABB(const BallObject& a, const GameObject& b);
};
