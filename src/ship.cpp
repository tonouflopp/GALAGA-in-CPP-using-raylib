#include "ship.hpp"
Ship::Ship()
{
    image = LoadTexture("Textures/spaceship.png");
    image.width = 50;
    image.height = 50;
    position.x = GetScreenWidth() / 2 - image.width / 2;
    position.y = GetScreenHeight() - image.height*4;
    lastlasertime = 0.0;
}

Ship::~Ship(){
    UnloadTexture(image);   
}

void Ship::Draw() {
    DrawTexture(image, position.x, position.y , WHITE);
}

void Ship::MoveLeft() {
    if (position.x > 0) {
        position.x -= 5;
    }
}
void Ship::MoveRight() {
    if (position.x < GetScreenWidth() - image.width) {
        position.x += 5;
    }
}

void Ship::fireLaser() {
    if (GetTime() - lastlasertime >= 0.5) {
    lasers.push_back(Laser({position.x + image.width / 2 - 2, position.y}, -1));
    lastlasertime = GetTime();
    }
}

Rectangle Ship::getRect() {
    return {position.x, position.y, (float)image.width, (float)image.height
    };
}

void Ship::Reset() 
{
position.x = GetScreenWidth() / 2 - image.width / 2;
position.y = GetScreenHeight() - image.height*4;
lasers.clear();    
}