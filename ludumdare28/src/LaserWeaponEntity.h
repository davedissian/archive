// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef PhantomLaserWeaponEntity_h__
#define PhantomLaserWeaponEntity_h__

#include "SpecialWeaponEntity.h"
#include "DameageableEntity.h"

class LaserWeaponEntity : public SpecialWeaponEntity,
                          public enable_shared_from_this<LaserWeaponEntity>
{
public:
    LaserWeaponEntity(const Vec2& startingPosition, weak_ptr<DamageableEntity> target);
    virtual ~LaserWeaponEntity();

    // Inherited from SpecialWeaponEntity
    virtual void update(float dt);
    virtual void render(sf::RenderWindow& window);
    virtual void onCollision(shared_ptr<Entity> other);
    virtual sf::Sprite& getSprite();

private:
    float mProgress;
    Vec2 mStartPosition;

    sf::Texture* mSquare;
    sf::Sprite mCollisionBox;
};

#endif    // PhantomLaserWeaponEntity_h__
