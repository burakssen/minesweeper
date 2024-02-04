#pragma once

#include <string>
#include <memory>
#include <chrono>

#include <raylib.h>

#include "Grid/Grid.h"

class Game
{
private:
    Game();

public:
    ~Game();
    static Game &GetInstance();
    void Run();

private:
    void Update();
    void Render();
    void HandleEvents();

private:
    int m_width = 1024;
    int m_height = 768;

    std::string m_title = "Mine Sweeper";
    Grid *m_grid = nullptr;
    std::shared_ptr<Camera2D> m_camera = std::make_shared<Camera2D>();

    bool m_gameOver = false;
    bool m_win = false;
    time_t m_startTime;
    time_t m_endTime;

    bool m_firstClick = false;
};