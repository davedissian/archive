#include "Common.h"
#include "Map.h"
#include "Track.h"
#include "Waypoint.h"

Map::Map(const string& mapFile)
{
    ifstream mapData(mapFile);
    Deserialise(mapData);
}

Map::~Map()
{
}

static char GetMapData(const string& buf, const Vec2i& pos, const Vec2u& size)
{
    // If the positions go off the edge, then assume it's a rail
    if (pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y)
        return '#';
    else
        return buf[pos.y * size.y + pos.x];
}

static bool IsTileTrack(const string& buf, const Vec2i& pos, const Vec2u& size)
{
    char c = GetMapData(buf, pos, size);
    return c == '#' || c == 'S' || c == 'E';
}

enum Direction { NORTH, EAST, SOUTH, WEST };

void Map::Deserialise(istream& in)
{
    int waypointCount, size;
    in >> waypointCount >> size;

    // Build map
    mWaypoint = make_shared<Waypoint>();
    in >> mStart.x >> mStart.y;
    mWaypoint->AddPosition(mStart);
    Vec2i prevDir;
    Vec2i a = mStart;
    Track::Direction endDir;
    for (int i = 1; i < waypointCount; i++)
    {
        Vec2i b;
        in >> b.x >> b.y;
        mWaypoint->AddPosition(b);
        if (i == (waypointCount - 1))
            mEnd = b;

        // Walk between points and place tracks

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

        auto getDir = [](const Vec2i& dir) -> Direction {
            if (dir.x == 0 && dir.y == -1)
                return NORTH;
            if (dir.x == 1 && dir.y == 0)
                return EAST;
            if (dir.x == 0 && dir.y == 1)
                return SOUTH;
            else
                return WEST;
        };

        // Add corner
        Track::Direction corner;
        if (a.x == mStart.x && a.y == mStart.y)
        {
            corner = dir.x == 0 ? Track::NS : Track::WE;
        }
        else
        {
            Direction prevDirC = getDir(-prevDir);
            Direction curDir = getDir(dir);
            if ((curDir == EAST && prevDirC == NORTH) ||
                (prevDirC == EAST && curDir == NORTH))
                corner = Track::NE;
            if ((curDir == SOUTH && prevDirC == EAST) ||
                (prevDirC == SOUTH && curDir == EAST))
                corner = Track::ES;
            if ((curDir == WEST && prevDirC == SOUTH) ||
                (prevDirC == WEST && curDir == SOUTH))
                corner = Track::SW;
            if ((curDir == NORTH && prevDirC == WEST) ||
                (prevDirC == NORTH && curDir == WEST))
                corner = Track::WN;
        }
        mTracks[a.y][a.x] = make_shared<Track>(corner, a);

        // Step distance - 1 times
        for (int s = 1; s < distance; s++)
        {
            Track::Direction d = dir.x == 0 ? Track::NS : Track::WE;
            Vec2i p = a + dir * s;
            mTracks[p.y][p.x] = make_shared<Track>(d, p);
            endDir = d;
        }

        prevDir = dir;
        a = b;
    }

    // Add end track
    // At this stage, a is the position of the last waypoint node
    mTracks[a.y][a.x] = make_shared<Track>(endDir, a);
    mWaypoint->DebugDump();
}

void Map::Serialise(ostream& out)
{
}

void Map::OnEvent(sf::Event& e)
{
    switch (e.type)
    {
    default:
        break;
    }
}

bool Map::Update(float dt)
{
    return true;
}

void Map::Draw()
{
    for (int y = 0; y < MAP_Y; y++)
        for (int x = 0; x < MAP_X; x++)
            if (mTracks[y][x])
                mTracks[y][x]->Draw();
}

