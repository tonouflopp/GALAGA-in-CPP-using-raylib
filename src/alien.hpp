#pragma once
#include <raylib.h>

class Aliens {
    public:
        Aliens(int type, Vector2 posision);
        void Update(int direction);
        void Draw();
        int GetType();
        static void UnloadImages();
        static Texture2D alienImages[3];
        int type;
        Vector2 position;
        Rectangle getRect();
        bool diving = false;

    private:
};