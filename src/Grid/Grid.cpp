#include "Grid.h"

#include <iostream>

Grid::Grid()
{
    this->m_texture = std::make_shared<Texture2D>(LoadTexture("resources/minesweeper.png"));

    this->m_explosionSound = std::make_shared<Sound>(LoadSound("resources/explosion.wav"));
    this->m_revealSound = std::make_shared<Sound>(LoadSound("resources/reveal.wav"));
    this->m_flagSound = std::make_shared<Sound>(LoadSound("resources/flag.wav"));
}

Grid::~Grid()
{
    UnloadTexture(*this->m_texture);
    UnloadSound(*this->m_explosionSound);
    UnloadSound(*this->m_revealSound);
    UnloadSound(*this->m_flagSound);
}

Grid &Grid::GetInstance()
{
    static Grid instance;
    return instance;
}

void Grid::Render()
{
    for (auto &row : this->m_cells)
    {
        for (auto &cell : row)
        {
            cell->Render();
        }
    }
}

void Grid::HandleEvents()
{
    for (auto &row : this->m_cells)
    {
        for (auto &cell : row)
        {
            cell->HandleEvents();
        }
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        this->m_currentDivisor++;
        if (this->m_currentDivisor >= 6)
        {
            this->m_currentDivisor = 3;
        }

        this->DestroyGrid();
        this->InitGrid();
    }

    if (IsKeyPressed(KEY_I))
    {
        this->DestroyGrid();
        this->m_mineRate += 0.05f;
        if (this->m_mineRate > 1.0f)
        {
            this->m_mineRate = 1.0f;
        }
        this->InitGrid();
    }

    if (IsKeyPressed(KEY_O))
    {
        this->DestroyGrid();
        this->m_mineRate -= 0.05f;

        if (this->m_mineRate < 0.15f)
        {
            this->m_mineRate = 0.15f;
        }

        this->InitGrid();
    }

    for (auto &row : this->m_cells)
    {
        for (auto &cell : row)
        {
            if (cell->GetState() != CellState::Revealed)
            {
                this->m_allCellsRevealed = false;
                return;
            }
        }
    }

    this->m_allCellsRevealed = true;
}

void Grid::SetNumOfCellsX(int numOfCellsX)
{
    this->m_numOfCellsX = numOfCellsX;
}

void Grid::SetNumOfCellsY(int numOfCellsY)
{
    this->m_numOfCellsY = numOfCellsY;
}

void Grid::SetCellSize(int cellSize)
{
    this->m_cellSize = cellSize;
}

void Grid::InitGrid()
{
    for (int i = 1; i <= this->m_size.x && i <= this->m_size.y; i++)
    {
        if ((int)this->m_size.x % i == 0 && (int)this->m_size.y % i == 0)
        {
            this->m_commonDivisors.emplace_back(i);
        }
    }

    this->m_cellSize = this->m_commonDivisors[this->m_currentDivisor];
    this->m_numOfCellsX = this->m_size.x / this->m_cellSize;
    this->m_numOfCellsY = this->m_size.y / this->m_cellSize;

    for (int i = 0; i < this->m_numOfCellsX; i++)
    {
        std::vector<std::shared_ptr<Cell>> row;
        for (int j = 0; j < this->m_numOfCellsY; j++)
        {
            auto cell = std::make_shared<Cell>(Vector2{(float)i * this->m_cellSize + this->m_position.x, (float)j * this->m_cellSize + this->m_position.y}, this->m_cellSize);
            cell->SetState(CellState::Hidden);
            cell->SetGrid(this);
            cell->SetExplosionSound(this->m_explosionSound);
            cell->SetFlagSound(this->m_flagSound);
            cell->SetRevealSound(this->m_revealSound);
            cell->SetTexture(this->m_texture);
            row.emplace_back(cell);
        }

        this->m_cells.emplace_back(row);
    }

    // set mines
    int mines = this->m_numOfCellsX * this->m_numOfCellsY * this->m_mineRate;

    for (int i = 0; i < mines; i++)
    {
        int index = GetRandomValue(0, this->m_numOfCellsX - 1);
        int index2 = GetRandomValue(0, this->m_numOfCellsY - 1);
        auto cell = this->m_cells[index][index2];
        cell->SetType(CellType::Mine);
    }

    for (int i = 0; i < this->m_numOfCellsX; i++)
    {
        for (int j = 0; j < this->m_numOfCellsY; j++)
        {
            auto cell = this->m_cells[i][j];
            int mines = 0;
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if (i + k >= 0 && i + k < this->m_numOfCellsX && j + l >= 0 && j + l < this->m_numOfCellsY)
                    {
                        if (this->m_cells[i + k][j + l]->GetType() == CellType::Mine)
                        {
                            mines++;
                        }
                    }

                    // add only the neighbours on the right, left, top and bottom
                    if (k == 0 && l == 0)
                    {
                        continue;
                    }

                    if (k != 0 && l != 0)
                    {
                        continue;
                    }

                    if (i + k >= 0 && i + k < this->m_numOfCellsX && j + l >= 0 && j + l < this->m_numOfCellsY)
                    {
                        cell->AddNeighbour(this->m_cells[i + k][j + l]);
                    }
                }
            }
            cell->SetNumberOfNeighbourMines(mines);
        }
    }
}

void Grid::DestroyGrid()
{
    this->m_cells.clear();
}

void Grid::SetPosition(Vector2 position)
{
    this->m_position = position;
}

void Grid::SetSize(Vector2 size)
{
    this->m_size = size;
}

std::string Grid::GetDimensions() const
{
    return "Grid dimensions: " + std::to_string(this->m_numOfCellsX) + "x" + std::to_string(this->m_numOfCellsY);
}

std::string Grid::GetMineRate() const
{
    return "Mine rate: " + std::to_string((int)(this->m_mineRate * 100.0f)) + "%";
}

void Grid::SetCellExploded(bool cellExploded)
{
    this->m_cellExploded = cellExploded;
}

bool Grid::GetCellExploded() const
{
    return this->m_cellExploded;
}

bool Grid::GetAllCellsRevealed() const
{
    return this->m_allCellsRevealed;
}