// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef EnemyPart_h__
#define EnemyPart_h__

#include "DameageableEntity.h"

class EnemyEntity;

class EnemyPartEntity : public DamageableEntity, public enable_shared_from_this<EnemyPartEntity>
{
public:
    EnemyPartEntity(EnemyPartDesc& desc, shared_ptr<EnemyEntity> parent);
    virtual ~EnemyPartEntity();

    weak_ptr<EnemyEntity> getParent() const;

    // Inherited from ShipEntity
    virtual void damage(const Vec2& direction, uint damageTaken);
    virtual void update(float dt);
    virtual void render(sf::RenderWindow& window);
    virtual void onCollision(shared_ptr<Entity> other);
    virtual sf::Sprite& getSprite();

protected:
    EnemyPartDesc& mDesc;

    weak_ptr<EnemyEntity> mParent;

    sf::Texture* mTexture;
    sf::Sprite mSprite;

    sf::Clock mBulletCycle;
};

#endif    // EnemyPart_h__
