#pragma once
#include <raylib.h>
#include "ship.hpp"
#include "alien.hpp"

class Game {
    public:
        Game();
        ~Game();
        void update();
        void HandleInput();
        bool run;
        void Draw();
        void DeleteLasers();
        bool getGameStart();
        void setGameStart();
        void GameOver();
        void Reset();
        void InitGame();
        int lives;
        int score;
        int highscore;
        float alienMoveTimer = 0.0F;
        void StartMenu(Music music, Texture2D image, Font font);

    private:
        std::vector<Aliens> CreateAliens();
        void MoveAliens();
        void AlienShootLaser();
        void CheckForCollisions();
        void checkForHighscore();
        void saveHighscoreToFile(int highscore);
        int loadHighscoreFromFile();
        Ship ship;
        std::vector<Aliens> aliens;
        int aliensDirection;
        std::vector<Laser> alienLasers;
        constexpr static float alienLaserShootInterval = 0.35f;
        constexpr static float alienShootDelay = 3.0f;
        float timeLastAlienFired;
        float gameTimer = 0.0f;
        bool gameStart = false;
        Sound explosionSound; 
};