#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Train;

class Bullet : public Entity
{
public:
    enum Type
    {
        Ball,
        Missile,
        Lightning
    };

    Bullet(const Vec2f& p, const Vec2f& v, weak_ptr<Train> target, Type type, float damage);
    ~Bullet();

    virtual void OnEvent(sf::Event&) {}
    virtual bool Update(float dt);
    virtual void Draw();

private:
    sf::Sprite mSprite;
    Vec2f mVelocity;
    float mSpeed;               // Used by missiles only
    Vec2f mAcceleration;        // Used by missiles only
    weak_ptr<Train> mTarget;
    Type mType;
    float mDamage;
};

#endif /* BULLET_H */
