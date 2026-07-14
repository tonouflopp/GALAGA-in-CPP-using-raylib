#pragma once
#include <raylib.h>
#include "laser.hpp"
#include <vector>

class Ship {
    public:
        Ship();
        ~Ship();
        void Update();
        void Draw();
        void MoveLeft();
        void MoveRight();
        void fireLaser();
        Rectangle getRect(); 
        void Reset();
        std::vector<Laser> lasers;

    private:
        Texture2D image;
        Vector2 position;
        float speed;
        Vector2 laserPosition;
        bool laserActive;
        double lastlasertime;
};
