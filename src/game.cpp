#include "game.hpp"
#include "alien.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

Game::Game() {
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0.0;
    explosionSound = LoadSound("Sounds/explosion.ogg");
    InitGame();
}

void Game::StartMenu(Music music, Texture2D titleimage, Font font) {
    UpdateMusicStream(music);
    DrawTextureV(titleimage, {float(GetScreenWidth()/4), float(GetScreenHeight()/2 - titleimage.height)}, WHITE);
    const char* Title = "Press ENTER to begin";
    DrawTextEx(font, Title, {float(GetScreenWidth()/2 - strlen(Title)*8), float(GetScreenHeight()/2 + 100)}, 34, 2, WHITE);
}

Game::~Game() {
    Aliens::UnloadImages();
    UnloadSound(explosionSound);
}

bool Game::getGameStart() {
    return gameStart;
}

void Game::update() {
    gameTimer += GetFrameTime();

    for (auto& laser : ship.lasers) {
        laser.Update();
    }
    MoveAliens();

    alienMoveTimer += GetFrameTime();
    if (alienMoveTimer >= 3.0f) {
        int index = GetRandomValue(0, aliens.size() - 1);
        aliens[index].diving = true;
        alienMoveTimer = 0;
    }

    AlienShootLaser();

    for (auto& laser : alienLasers) {
        laser.Update();
    }

    DeleteLasers();
    CheckForCollisions();

}

void Game::Draw() {
    ship.Draw();

    for (auto& laser : ship.lasers) {
        laser.Draw();
    }
    for (auto& alien : aliens) {
        alien.Draw();
    }
    for (auto& laser : alienLasers) {
        laser.Draw();
    }
}

void Game::HandleInput() {
    if (run) {
        if (IsKeyDown(KEY_LEFT)) {
            ship.MoveLeft();
        } else if (IsKeyDown(KEY_RIGHT)) {
            ship.MoveRight();
        } else if (IsKeyPressed(KEY_SPACE)) {
            ship.fireLaser();
        } else if (IsKeyPressed(KEY_ENTER)) {
            this->gameStart = true;
        }
    } else {
        if (IsKeyPressed(KEY_DOWN)) {
            Reset();
            InitGame();
        }
    }
}

void Game::DeleteLasers() {
    for (auto it = ship.lasers.begin(); it != ship.lasers.end();) {
        if (!it->active) it = ship.lasers.erase(it);
        else ++it;
    }
    for (auto it = alienLasers.begin(); it != alienLasers.end();) {
        if (!it->active) it = alienLasers.erase(it);
        else ++it;
    }
}

std::vector<Aliens> Game::CreateAliens() {
    std::vector<Aliens> aliens;
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 11; column++) {
            int alienType;
            if (row == 0) alienType = 3;
            else if (row == 1 || row == 2) alienType = 2;
            else alienType = 1;

            float x = 75 + column * 55;
            float y = 110 + row * 55;
            aliens.push_back(Aliens(alienType, {x, y}));
        }
    }
    return aliens;
}

void Game::MoveAliens() {
    for (auto& a : aliens) {
        if (a.diving) a.position.y += 3;
    }
    for (auto& a : aliens) {
        if (a.position.x + a.alienImages[a.type - 1].width > float(GetScreenWidth()))
            aliensDirection = -1;
        if (a.position.x < 0)
            aliensDirection = 1;
        a.Update(aliensDirection);              
    }
}

void Game::AlienShootLaser() {
    if (gameTimer < alienShootDelay) return;

    double currentTime = GetTime();
    if (currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty() && GetTime() > 0) {
        std::vector<int> divingIndices;
        for (int i = 0; i < (int)aliens.size(); i++) {
            if (aliens[i].diving) divingIndices.push_back(i);
        }

        int randomIndex;
        if (!divingIndices.empty()) {
            randomIndex = divingIndices[GetRandomValue(0, divingIndices.size() - 1)];
        } else {
            randomIndex = GetRandomValue(0, aliens.size() - 1);
        }

        LaserPattern patterns[] = {
            LaserPattern::STRAIGHT,
            LaserPattern::DIAGONAL_LEFT,
            LaserPattern::DIAGONAL_RIGHT,
            LaserPattern::ZIGZAG
        };
        LaserPattern chosenPattern = patterns[GetRandomValue(0, 3)];

        Aliens& a = aliens[randomIndex];
        alienLasers.push_back(Laser(
            {a.position.x + a.alienImages[a.type - 1].width / 2,
             a.position.y + a.alienImages[a.type - 1].height},
            1,
            chosenPattern
        ));
        timeLastAlienFired = GetTime();
    }
}

void Game::CheckForCollisions() {
    for (auto& laser : ship.lasers) {
        auto it = aliens.begin();
        while (it != aliens.end()) {
            if (CheckCollisionRecs(it->getRect(), laser.getRect())) 
            {
                
                PlaySound(explosionSound);
                it = aliens.erase(it);
                laser.active = false;
                score += 200;
            } else {
                ++it;
            }
        }
    }

    for (auto& laser : alienLasers) {
        if (CheckCollisionRecs(laser.getRect(), ship.getRect())) {
            laser.active = false;
            lives--;
            if (lives == 0) GameOver();
        }
    }

    for (auto it = aliens.begin(); it != aliens.end();) {
        if (it->diving && CheckCollisionRecs(it->getRect(), ship.getRect())) {
            it = aliens.erase(it);
            lives--;
            if (lives == 0) GameOver();
        } else {
            ++it;
        }
    }
    if(aliens.empty()) {
    GameOver();
}
}

void Game::GameOver() {
    checkForHighscore();
    run = false;
}

void Game::InitGame() {
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0.0;
    lives = 3;
    run = true;
    score = 0;
    gameTimer = 0.0f;
    highscore = loadHighscoreFromFile();
}

void Game::checkForHighscore() {
    if (score > highscore) {
        highscore = score;
        saveHighscoreToFile(highscore);
    }
}

void Game::saveHighscoreToFile(int highscore) {
    std::ofstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open()) {
        highscoreFile << highscore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

int Game::loadHighscoreFromFile() {
    int loadedHighscore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open()) {
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to load highscore from file." << std::endl;
    }
    return loadedHighscore;
}

void Game::Reset() {
    ship.Reset();
    aliens.clear();
    alienLasers.clear();
}
