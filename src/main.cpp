#include <raylib.h>
#include "game.hpp"
#include <string>
#include <cstring>

std::string FormatWithLeadingZeros(int number, int width) {
    std::string numberText = std::to_string(number);
    int leadingZeros = width - (width/2 + numberText.length()/2);
    return numberText = std::string(leadingZeros, ' ') + numberText;
}

int main()
{
    Color red = {255,0,0,255};
    int offset = 50;
    const int windowWidth = 800;
    const int windowHeight = 1000;

    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Galaga");
    InitAudioDevice();


    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Textures/spaceship.png");
    spaceshipImage.width = 50;
    spaceshipImage.height = 50;
    SetTargetFPS(60);
    Texture2D titleImage = LoadTexture("Textures/galaga.png");
    titleImage.width = 400;
    titleImage.height = 350;
    Game game;
    Music music = LoadMusicStream("sounds/galaga-theme.ogg");        
    PlayMusicStream(music);
    
    while(WindowShouldClose() == false) {
        game.HandleInput();
        game.update();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRoundedLines({10, 10, 780}, 0.18f, 20, WHITE );
        
        if (!game.getGameStart()) {
            game.StartMenu(music,titleImage,font);
        }
        else {
        if(!game.run){
            DrawTextEx(font, "GAME OVER", {windowWidth/3, windowHeight/2}, 34, 2, red);
            DrawTextEx(font, "Restart?   Press DOWN", {windowWidth/3, windowHeight/2 + 40}, 34, 2, WHITE   );
        }
        StopMusicStream(music);
        
        float x=0.0;
        if (game.lives <= 3) {
        for(int i = 0; i < game.lives-1; i ++, x+=50.0) {
            DrawTextureV(spaceshipImage, {10+x, float(windowHeight-20)}, WHITE);
        }}

        std::string scoreText = FormatWithLeadingZeros(game.highscore, windowWidth);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, red);
        const char* hscore = "HIGH-SCORE";
        DrawTextEx(font, hscore, {float(windowWidth/2 - strlen(hscore) ),10}, 34, 2, red);
        std::string highscoreText = FormatWithLeadingZeros(game.highscore, 10);
        DrawTextEx(font, highscoreText.c_str(), {windowWidth/2, 40}, 34, 2, WHITE);

        game.Draw();
       } EndDrawing();
    }
    

    CloseWindow();
    CloseAudioDevice();
}