#include "Common.h"
#include "Waypoint.h"

Waypoint::Waypoint()
{
}

Waypoint::~Waypoint()
{
}

void Waypoint::AddPosition(const Vec2i& p)
{
    mNodes.emplace_back(p);
}

void Waypoint::DebugDump()
{
    int size = 12;
    char grid[size][size];
    memset(grid, ' ', size * size);

    // Draw between nodes
    for (auto i = mNodes.begin(); i != mNodes.end() - 1; i++)
    {
        auto& a = i->mPosition;
        auto& b = (i + 1)->mPosition;

        // Calculate direction
        Vec2i dir = b - a;
        int distance = (int)sqrt(dir.x * dir.x + dir.y * dir.y);
        if (dir.x > 1)
            dir.x = 1;
        if (dir.x < -1)
            dir.x = -1;
        if (dir.y > 1)
            dir.y = 1;
        if (dir.y < -1)
            dir.y = -1;

        // Mark starting position
        grid[a.y][a.x] = 'X';

        // Step distance - 1 times
        for (int s = 1; s < distance; s++)
            grid[a.y + dir.y * s][a.x + dir.x * s] = dir.x == 0 ? '|' : '-';
    }

    // Display grid
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
            cout << grid[r][c];
        cout << endl;
    }
}

Waypoint::NodeIterator Waypoint::begin()
{
    return mNodes.begin();
}

Waypoint::NodeIterator Waypoint::end()
{
    return mNodes.end();
}

