#include "alien.hpp"

Texture2D Aliens::alienImages[3] = {};

Aliens::Aliens(int type, Vector2 position) 
{
    this -> type = type;
    this -> position = position;

    if(alienImages[type -1].id == 0) {

    switch (type) {
        case 1:
            alienImages[0] = LoadTexture("Textures/fly.png");
            break;
        case 2:
            alienImages[1] = LoadTexture("Textures/Butterfly.png");
            break;
        case 3:
            alienImages[2] = LoadTexture("Textures/2hitBug.png");
            break;
        default:
            alienImages[0] = LoadTexture("Textures/fly.png");
            break;
    }
        alienImages[type-1].width = 50;
        alienImages[type-1].height = 50;
    }
    
}

void Aliens::Draw() {
    DrawTextureV(alienImages[type-1], position, WHITE);
}

int Aliens::GetType() {
    return type;
}

void::Aliens::UnloadImages() 
{
    for(int i = 0; i < 4; i++) {
        UnloadTexture(alienImages[i]);
    }
}

Rectangle Aliens::getRect()
{
    return{position.x, position.y,
    float(alienImages[type - 1].width),
    float(alienImages[type - 1].height)
    };
}

void::Aliens::Update(int direction) {
    position.x += direction;
}