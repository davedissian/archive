#pragma once

#include "SpecialWeaponEntity.h"
#include "DameageableEntity.h"

class MissileWeaponEntity : public SpecialWeaponEntity
{
public:
    MissileWeaponEntity(const Vec2& startingPosition, weak_ptr<DamageableEntity> target);
    virtual ~MissileWeaponEntity();

    // Inherited from SpecialWeapon
    virtual void update(float dt);
    virtual void render(sf::RenderWindow& window);
    virtual void onCollision(shared_ptr<Entity> other);
    virtual sf::Sprite& getSprite();

private:
    // Rendering
    sf::Texture* mTexture;
    sf::Sprite mSprite;
};
