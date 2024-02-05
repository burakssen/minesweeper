#pragma once

#include <memory>
#include <vector>

#include <raylib.h>

enum class CellState
{
    Hidden,
    Revealed,
    Flagged
};

enum class CellType
{
    Empty,
    Number,
    Mine
};

#include "Grid.h"

class Grid;

class Cell
{
public:
    Cell(Vector2 position, int size);
    ~Cell();

    void Render();
    void HandleEvents();
    void SetState(CellState state);
    void SetType(CellType type);

    CellState GetState() const;
    CellType GetType() const;

    void SetTexture(std::shared_ptr<Texture2D> texture);
    void SetSourceRec(Vector2 tilePos);
    void SetNumberOfNeighbourMines(int numberOfNeighbourMines);

    void AddNeighbour(std::shared_ptr<Cell> cell);

    void RevealNeighbours(int depth);

    void SetGrid(Grid *grid);

private:
    Vector2 m_position;
    int m_size;

    CellState m_state = CellState::Hidden;
    CellType m_type = CellType::Number;

    std::shared_ptr<Texture2D> m_texture;
    Rectangle m_sourceRec;
    int m_numberOfNeighbourMines = 0;

    float m_time = 0.0f;
    bool m_exploded = false;

    std::vector<std::shared_ptr<Cell>> m_neighbours;

    Grid *m_grid = nullptr;

    std::shared_ptr<Sound> m_explosionSound;
    std::shared_ptr<Sound> m_flagSound;
    std::shared_ptr<Sound> m_revealSound;
};