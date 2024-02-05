#include "Cell.h"

#include <iostream>

Cell::Cell(Vector2 position, int size)
    : m_position(position), m_size(size)
{
    this->m_explosionSound = std::make_shared<Sound>(LoadSound("resources/explosion.wav"));
    this->m_revealSound = std::make_shared<Sound>(LoadSound("resources/reveal.wav"));
    this->m_flagSound = std::make_shared<Sound>(LoadSound("resources/flag.wav"));
}

Cell::~Cell()
{
}

void Cell::Render()
{
    Rectangle destRec = {m_position.x, m_position.y, (float)this->m_size, (float)this->m_size};
    Vector2 origin = {0.0f, 0.0f};

    if (this->m_state == CellState::Hidden)
    {
        this->SetSourceRec({2, 3});
        DrawTexturePro(*m_texture, this->m_sourceRec, destRec, origin, 0.0f, WHITE);
    }
    else if (this->m_state == CellState::Revealed)
    {
        if (this->m_type == CellType::Empty)
        {
            this->SetSourceRec({3, 3});
            DrawTexturePro(*m_texture, this->m_sourceRec, destRec, origin, 0.0f, WHITE);
        }
        else if (this->m_type == CellType::Number)
        {
            this->SetSourceRec({3, 3});
            DrawTexturePro(*m_texture, this->m_sourceRec, destRec, origin, 0.0f, WHITE);
            if (0 < this->m_numberOfNeighbourMines && this->m_numberOfNeighbourMines < 5)
                this->SetSourceRec({(float)this->m_numberOfNeighbourMines - 1, 1});
            else if (9 > this->m_numberOfNeighbourMines && this->m_numberOfNeighbourMines > 5)
            {
                this->SetSourceRec({(float)this->m_numberOfNeighbourMines - 1, 2});
            }
            DrawTexturePro(*m_texture, this->m_sourceRec, destRec, origin, 0.0f, WHITE);
        }
        else if (this->m_type == CellType::Mine)
        {
            if (!this->m_exploded)
            {
                this->m_time += GetFrameTime();
                if (this->m_time < 0.7f)
                    this->SetSourceRec({0, 0});
                else if (this->m_time < 1.4f)
                    this->SetSourceRec({1, 0});
                else if (this->m_time < 2.1f)
                {

                    PlaySound(*m_explosionSound);
                    this->SetSourceRec({3, 0});
                    this->m_exploded = true;
                }
            }

            DrawTexturePro(*m_texture, this->m_sourceRec, destRec, origin, 0.0f, WHITE);
        }
    }
    else if (this->m_state == CellState::Flagged)
    {

        this->SetSourceRec({2, 3});
        DrawTexturePro(*m_texture, this->m_sourceRec, destRec, origin, 0.0f, WHITE);
        this->SetSourceRec({2, 0});
        DrawTexturePro(*m_texture, this->m_sourceRec, destRec, origin, 0.0f, WHITE);
    }
}

void Cell::HandleEvents()
{

    if (this->m_state != CellState::Revealed)
    {
        Vector2 mousePos = GetMousePosition();
        Rectangle cellRec = {m_position.x, m_position.y, (float)m_size, (float)m_size};

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mousePos, cellRec))
            {
                this->m_state = CellState::Revealed;
                if (this->m_type != CellType::Mine)
                {
                    PlaySound(*m_revealSound);
                    this->RevealNeighbours(0);
                }
                else
                {
                    if (this->m_grid != nullptr)
                    {
                        this->m_grid->SetCellExploded(true);
                    }
                }
            }
        }
        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            if (CheckCollisionPointRec(mousePos, cellRec))
            {
                if (this->m_state == CellState::Hidden)
                {
                    PlaySound(*m_flagSound);
                    this->m_state = CellState::Flagged;
                }
                else if (this->m_state == CellState::Flagged)
                {
                    PlaySound(*m_flagSound);
                    this->m_state = CellState::Hidden;
                }
            }
        }
    }
}

void Cell::SetState(CellState state)
{
    this->m_state = state;
}

void Cell::SetType(CellType type)
{
    this->m_type = type;
}

CellState Cell::GetState() const
{
    return this->m_state;
}

CellType Cell::GetType() const
{
    return this->m_type;
}

void Cell::SetTexture(std::shared_ptr<Texture2D> texture)
{
    this->m_texture = texture;
}

void Cell::SetSourceRec(Vector2 tilePos)
{
    this->m_sourceRec = {
        tilePos.x * this->m_texture->width / 4, tilePos.y * this->m_texture->width / 4, (float)m_texture->width / 4, (float)m_texture->height / 4};
}

void Cell::SetNumberOfNeighbourMines(int numberOfNeighbourMines)
{
    this->m_numberOfNeighbourMines = numberOfNeighbourMines;
}

void Cell::AddNeighbour(std::shared_ptr<Cell> cell)
{
    this->m_neighbours.push_back(cell);
}

void Cell::RevealNeighbours(int depth)
{
    if (depth > 5)
        return;

    for (auto &neighbour : this->m_neighbours)
    {
        if (neighbour->GetState() == CellState::Hidden && neighbour->GetType() != CellType::Mine)
        {
            neighbour->SetState(CellState::Revealed);
            if (neighbour->m_numberOfNeighbourMines == 0)
            {
                neighbour->RevealNeighbours(depth + 1);
            }
        }
    }
}

void Cell::SetGrid(Grid *grid)
{
    this->m_grid = grid;
}
