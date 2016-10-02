// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef EnemyEntity_h__
#define EnemyEntity_h__

#include "DameageableEntity.h"
#include "EnemyPartEntity.h"
#include "EntityManager.h"

class EnemyEntity : public DamageableEntity, public enable_shared_from_this<EnemyEntity>
{
public:
    EnemyEntity(const Vec2& position, EnemyDesc& desc);
    virtual ~EnemyEntity();

    // Sub-parts
    void _setupParts();
    void _onDestroy();

    // Inherited from ShipEntity
    virtual void damage(const Vec2& direction, uint damageTaken);
    virtual void update(float dt);
    virtual void render(sf::RenderWindow& window);
    virtual void onCollision(shared_ptr<Entity> other);
    virtual sf::Sprite& getSprite();

private:
    EnemyDesc mDesc;

    float mTimer;
    Vec2 mSpeed;

    // Sub parts
    vector<shared_ptr<EnemyPartEntity>> mParts;

    sf::Texture* mTexture;    // TODO handle this in a texture manager
    sf::Sprite mSprite;
};

#endif    // EnemyEntity_h__
