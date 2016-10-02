#ifndef TRACK_H
#define TRACK_H

#include "Entity.h"

class Track : public Entity
{
public:
    enum Direction
    {
        NS = 0,     // top-bottom
        WE,         // left-right
        
        NE,
        ES,
        SW,
        WN
    };

    Track(Direction dir, const Vec2i& position);
    ~Track();

    virtual void OnEvent(sf::Event&) {}
    virtual bool Update(float dt) { return true; }
    virtual void Draw();

    Direction GetDirection() const { return mDirection; }

private:
    sf::Sprite mSprite;
    Direction mDirection;

};

#endif /* TRACK_H */
