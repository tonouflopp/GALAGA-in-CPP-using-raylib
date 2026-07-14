#pragma once
#include <raylib.h>

enum class LaserPattern { STRAIGHT, DIAGONAL_LEFT, DIAGONAL_RIGHT, ZIGZAG };

class Laser {
    public:
        Laser(Vector2 position, int direction, LaserPattern pattern = LaserPattern::STRAIGHT);
        void Update();
        void Draw();
        Rectangle getRect();
        bool active;

    private:
        Vector2 position;
        int speed;
        int direction;
        float speedX;
        float zigzagTimer;
        LaserPattern pattern;
};