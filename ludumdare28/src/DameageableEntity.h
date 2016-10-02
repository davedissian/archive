// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef Ship_h__
#define Ship_h__

#include "Entity.h"

class DamageableEntity : public Entity
{
public:
    DamageableEntity(const Vec2& position, int health, bool lockable);
    virtual ~DamageableEntity();

    int getHealth() const;
    bool isLockable() const;

    // Inherited from Entity
    virtual void damage(const Vec2& direction, uint damageTaken);
    virtual void onCollision(shared_ptr<Entity> other);

protected:
    int mHealth;
    bool mLockable;
};

#endif    // Ship_h__
