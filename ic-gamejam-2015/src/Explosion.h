#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Entity.h"

class Explosion : public Entity
{
public:
    Explosion(const Vec2f& pixelPosition, bool large);
    ~Explosion();

    virtual void OnEvent(sf::Event&) {}
    virtual bool Update(float dt);
    virtual void Draw();

private:
    float mTimeSinceLastFrame;
    int mSize;
    int mFrame;
    int mMaxFrames;
    sf::Sprite mSprite;
};

#endif /* EXPLOSION_H */
