#include "laser.hpp"

Laser::Laser(Vector2 position, int direction, LaserPattern pattern)
{
    this->position = position;
    speed = 5;
    active = true;
    this->direction = direction;
    this->pattern = pattern;
    zigzagTimer = 0.0f;

    switch (pattern) {
        case LaserPattern::DIAGONAL_LEFT:  speedX = -2.0f; break;
        case LaserPattern::DIAGONAL_RIGHT: speedX =  2.0f; break;
        case LaserPattern::ZIGZAG:         speedX = (GetRandomValue(0, 1) == 0) ? 2.0f : -2.0f; break;
        default:                           speedX =  0.0f; break;
    }
}

void Laser::Draw() {
    if (active) {
        DrawRectangle(position.x, position.y, 5, 5, WHITE);
    }
}

Rectangle Laser::getRect() {
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 4;
    rect.height = 15;
    return rect;
}

void Laser::Update() {
    if (pattern == LaserPattern::ZIGZAG) {
        zigzagTimer += GetFrameTime();
        if (zigzagTimer >= 0.3f) {
            speedX = -speedX;
            zigzagTimer = 0.0f;
        }
    }

    position.x += speedX;
    position.y += speed * direction;

    if (position.y < 0 || position.y > GetScreenHeight()) {
        active = false;
    }
}