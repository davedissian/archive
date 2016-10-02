// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EnemyPartEntity.h"
#include "EnemyEntity.h"
#include "EntityManager.h"
#include "BulletManager.h"

EnemyPartEntity::EnemyPartEntity(EnemyPartDesc& desc, shared_ptr<EnemyEntity> parent)
    : DamageableEntity(desc.position, desc.health, desc.lockable), mDesc(desc), mParent(parent)
{
    if (desc.sprite.length() > 0)
    {
        mTexture = ResourceCache::inst().getTexture(desc.sprite);
        Vec2 textureSize((float)mTexture->getSize().x, (float)mTexture->getSize().y);
        mSprite.setTexture(*mTexture);
        mSprite.setOrigin(textureSize * 0.5f);
    }
    mSprite.setRotation(desc.rotation);
}

EnemyPartEntity::~EnemyPartEntity()
{
}

weak_ptr<EnemyEntity> EnemyPartEntity::getParent() const
{
    return mParent;
}

void EnemyPartEntity::damage(const Vec2& direction, uint damageTaken)
{
    DamageableEntity::damage(direction, damageTaken);
}

void EnemyPartEntity::update(float dt)
{
    shared_ptr<EnemyEntity> parent = mParent.lock();
    assert(parent != shared_ptr<EnemyEntity>());

    Vec2 worldPosition = mPosition + parent->getPosition();

    Vec2 playerDirection;
    if (EntityManager::inst().getPlayer() != shared_ptr<PlayerEntity>())
        playerDirection = worldPosition - EntityManager::inst().getPlayer()->getPosition();
    else
        playerDirection = Vec2(0.0f, -1.0f);

    // Aim at the player
    if (!mDesc.fixed)
    {
        float rotation = atan2(playerDirection.y, playerDirection.x) * RAD_TO_DEG - 90.0f;
        mSprite.setRotation(rotation + mDesc.rotation);
    }

    // Fire bullets
    Vec2 velocity(sin(mSprite.getRotation() * DEG_TO_RAD),
                  -cos(mSprite.getRotation() * DEG_TO_RAD));
    velocity *= mDesc.bulletSpeed;
    if (worldPosition.y >= 0.0f && worldPosition.y < Game::SCREEN_HEIGHT)
    {
        if (mBulletCycle.getElapsedTime().asSeconds() > mDesc.bulletInterval / Game::getTimeRate())
        {
            BulletManager::inst().spawn(worldPosition, velocity, parent, BT_ENEMY_COMMON, false);
            mBulletCycle.restart();
        }
    }
}

void EnemyPartEntity::render(sf::RenderWindow& window)
{
    shared_ptr<EnemyEntity> parent = mParent.lock();
    assert(parent != shared_ptr<EnemyEntity>());

    // Calculate world position
    Vec2 worldPosition = mPosition + parent->getPosition();
    mSprite.setPosition(worldPosition);
    window.draw(mSprite);
}

void EnemyPartEntity::onCollision(shared_ptr<Entity> other)
{
    DamageableEntity::onCollision(other);

    // If we're dead - remove ourselves
    if (mHealth <= 0)
        EntityManager::inst().destroyEntity(shared_from_this());
}

sf::Sprite& EnemyPartEntity::getSprite()
{
    return mSprite;
}
