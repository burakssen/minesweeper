#pragma once

#include <memory>
#include <vector>
#include <string>

#include <raylib.h>

#include "Cell.h"

class Cell;

class Grid
{
private:
    Grid();

public:
    ~Grid();
    static Grid &GetInstance();

    void Render();
    void HandleEvents();

    void SetNumOfCellsX(int numOfCellsX);
    void SetNumOfCellsY(int numOfCellsY);
    void SetCellSize(int cellSize);

    void InitGrid();
    void DestroyGrid();

    void SetPosition(Vector2 position);
    void SetSize(Vector2 size);

    std::string GetDimensions() const;
    std::string GetMineRate() const;

    void SetCellExploded(bool cellExploded);
    bool GetCellExploded() const;

    bool GetAllCellsRevealed() const;

private:
    Vector2 m_position = {0, 0};
    Vector2 m_size = {0, 0};

    int m_numOfCellsX = 0;
    int m_numOfCellsY = 0;
    int m_cellSize = 0;

    float m_mineRate = 0.15f;

    std::shared_ptr<Texture2D> m_texture;
    std::vector<std::vector<std::shared_ptr<Cell>>> m_cells;

    std::vector<int> m_commonDivisors;
    int m_currentDivisor = 5;

    bool m_cellExploded = false;

    bool m_allCellsRevealed = false;
};