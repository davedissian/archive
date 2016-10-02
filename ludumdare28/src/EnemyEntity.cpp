// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EnemyEntity.h"
#include "EntityManager.h"
#include "Renderer.h"

EnemyEntity::EnemyEntity(const Vec2& position, EnemyDesc& desc)
    : DamageableEntity(position, desc.health, true), mDesc(desc), mTimer(0.0f)
{
    if (desc.timeline.size() > 0)
        mSpeed = desc.timeline.front().speed;
    else
        mSpeed = desc.speed;

    // TODO: ERROR CHECKING
    mTexture = ResourceCache::inst().getTexture(desc.sprite);
    Vec2 textureSize((float)mTexture->getSize().x, (float)mTexture->getSize().y);
    mSprite.setTexture(*mTexture);
    mSprite.setOrigin(textureSize * 0.5f);
}

EnemyEntity::~EnemyEntity()
{
    mParts.clear();
}

void EnemyEntity::_setupParts()
{
    for (auto i = mDesc.parts.begin(); i != mDesc.parts.end(); ++i)
    {
        shared_ptr<EnemyPartEntity> part =
            EntityManager::inst().createEnemyPart(*i, shared_from_this());
        mParts.push_back(part);
    }
}

void EnemyEntity::_onDestroy()
{
    for (auto i = mParts.begin(); i != mParts.end(); ++i)
        EntityManager::inst().destroyEntity(*i);
    mParts.clear();
}

void EnemyEntity::damage(const Vec2& direction, uint damageTaken)
{
    DamageableEntity::damage(direction, damageTaken);
}

void EnemyEntity::update(float dt)
{
    mTimer += dt;

    // Apply timeline actions
    if (mTimer && mDesc.timeline.size() > 0)
    {
        while (mTimer > mDesc.timeline.front().time)
        {
            mSpeed = mDesc.timeline.front().speed;
            mDesc.timeline.pop_front();
            if (mDesc.timeline.size() == 0)
                break;
        }
    }

    mPosition += mSpeed * dt;
}

void EnemyEntity::render(sf::RenderWindow& window)
{
    mSprite.setPosition(mPosition);
    window.draw(mSprite);
}

void EnemyEntity::onCollision(shared_ptr<Entity> other)
{
    DamageableEntity::onCollision(other);

    // If we're dead - remove ourselves
    if (mHealth <= 0)
    {
        Game::score += mTexture->getSize().x;    // Larger entities = more points

        EntityManager::inst().destroyEntity(shared_from_this());

        // Spawn explosion
        Renderer::inst().createExplosion(mPosition, (float)mTexture->getSize().x);
    }
}

sf::Sprite& EnemyEntity::getSprite()
{
    return mSprite;
}
