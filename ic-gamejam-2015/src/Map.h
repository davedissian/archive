#ifndef MAP_H
#define MAP_H

#include "Entity.h"

class Track;
class Waypoint;

class Map : public Entity
{
public:
    Map(const string& mapFile);
    ~Map();

    void Deserialise(istream& in);
    void Serialise(ostream& out);

    virtual void OnEvent(sf::Event& e);
    virtual bool Update(float dt);
    virtual void Draw();

    shared_ptr<Track> TrackAt(const Vec2i& p) { return mTracks[p.y][p.x]; }

    const Vec2i& GetStart() const { return mStart; }
    const Vec2i& GetEnd() const { return mEnd; }
    Waypoint* GetWaypoint() const { return mWaypoint.get(); }

private:
    Vec2i mStart, mEnd;
    shared_ptr<Waypoint> mWaypoint;
    shared_ptr<Track> mTracks[MAP_Y][MAP_X];
};

#endif /* MAP_H */
