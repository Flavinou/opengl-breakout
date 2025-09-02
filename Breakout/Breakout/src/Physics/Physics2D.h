#pragma once

#include "BallObject.h"
#include "GameObject.h"

class Physics2D
{
public:
    static bool CheckCollisionAABB(const GameObject& a, const GameObject& b);
    static bool CheckCollisionCircleAABB(const BallObject& a, const GameObject& b);
};
