#include "Game.h"

#include <iostream>

Game::Game()
{
    InitWindow(this->m_width, m_height, m_title.c_str());
    SetTargetFPS(60);

    InitAudioDevice();

    this->m_grid = &Grid::GetInstance();
    this->m_grid->SetPosition({0.0f, (float)this->m_height - 672});
    this->m_grid->SetSize({(float)1024, 672});
    this->m_grid->InitGrid();

    this->m_camera->target = {0, 0};
    this->m_camera->offset = {0.0f, 0.0f};
    this->m_camera->rotation = 0.0f;
    this->m_camera->zoom = 1.0f;
}

Game::~Game()
{
    CloseAudioDevice();
    this->m_grid->DestroyGrid();
    CloseWindow();
}

Game &Game::GetInstance()
{
    static Game instance;
    return instance;
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        this->HandleEvents();
        this->Update();
        this->Render();
    }
}

void Game::Update()
{
    if (this->m_grid->GetCellExploded() && !this->m_gameOver)
    {
        this->m_gameOver = true;
        this->m_endTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

    if (this->m_grid->GetAllCellsRevealed() && !this->m_win)
    {
        this->m_win = true;
        this->m_endTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }
}

void Game::Render()
{
    BeginDrawing();
    ClearBackground(BLACK);
    if (this->m_gameOver)
    {
        DrawText("Game Over", 10, 10, 20, WHITE);
        DrawText("Press R to restart", 10, 30, 20, WHITE);
    }
    else if (this->m_win)
    {
        DrawText("You Win", 10, 10, 20, WHITE);
        DrawText("Press R to restart", 10, 30, 20, WHITE);
    }
    else
    {
        DrawText("Press SPACE to change the number of cells", 10, 10, 20, WHITE);
        DrawText("Press I/O to change the mine rate", 10, 30, 20, WHITE);
        DrawText(this->m_grid->GetMineRate().c_str(), 10, 50, 20, WHITE);
        DrawText(this->m_grid->GetDimensions().c_str(), 10, 70, 20, WHITE);
    }

    BeginMode2D(*this->m_camera);
    this->m_grid->Render();
    EndMode2D();
    EndDrawing();
}

void Game::HandleEvents()
{

    if (!this->m_firstClick)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            this->m_firstClick = true;
            this->m_startTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        }
    }

    // Camera controls are disabled because it doesn't work well with the grid
    /*if (int wm = GetMouseWheelMove(); wm != 0)
    {
        this->m_camera->zoom += wm * 0.05f;

        if (this->m_camera->zoom < 1.0f)
        {
            this->m_camera->zoom = 1.0f;

            this->m_camera->offset.x = 0.0f;
            this->m_camera->offset.y = 0.0f;

            this->m_camera->target = {0, 0};
        }
        else if (this->m_camera->zoom > 3.0f)
        {
            this->m_camera->zoom = 3.0f;
        }

        this->m_camera->offset.x = (float)(this->m_width - this->m_width * this->m_camera->zoom) * 0.5f;
        this->m_camera->offset.y = (float)(this->m_height - this->m_height * this->m_camera->zoom) * 0.5f;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        // drag the camera until edge of the grid
        Vector2 mousePos = GetMousePosition();
        // check if mouse dragging the camera
        if (this->m_camera->target.x != 0 || this->m_camera->target.y != 0)
        {
            this->m_camera->offset.x += (mousePos.x - this->m_camera->target.x) * 0.5f;
            this->m_camera->offset.y += (mousePos.y - this->m_camera->target.y) * 0.5f;
        }
    }*/

    if (IsKeyPressed(KEY_R))
    {
        this->m_gameOver = false;
        this->m_grid->DestroyGrid();
        this->m_grid->InitGrid();
        this->m_grid->SetCellExploded(false);
    }

    if (!this->m_gameOver)
        this->m_grid->HandleEvents();
}