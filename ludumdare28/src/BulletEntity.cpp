// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "BulletEntity.h"

BulletEntity::BulletEntity()
    : Entity(Vec2(0.0f, 0.0f)),
      mVelocity(0.0f, 0.0f),
      mRotation(0.0f),
      mParent(),
      mActive(false),
      mTexture(nullptr)
{
}

BulletEntity::~BulletEntity()
{
}

void BulletEntity::spawn(const Vec2& position, const Vec2& velocity, shared_ptr<Entity> parent,
                         sf::Texture* texture)
{
    mPosition = position;
    mVelocity = velocity;
    mRotation = atan2(velocity.y, velocity.x) * RAD_TO_DEG + 90.0f;
    mParent = parent;

    Vec2 textureSize((float)texture->getSize().x, (float)texture->getSize().y);
    mTexture = texture;
    mSprite.setTexture(*mTexture);
    mSprite.setOrigin(textureSize * 0.5f);
    mSprite.setRotation(mRotation);

    mActive = true;
}

void BulletEntity::despawn()
{
    mActive = false;
}

void BulletEntity::update(float dt)
{
    mPosition += mVelocity * dt;
}

void BulletEntity::render(sf::RenderWindow& window)
{
    if (mActive)
    {
        mSprite.setPosition(mPosition);
        window.draw(mSprite);
    }
}

void BulletEntity::onCollision(shared_ptr<Entity> other)
{
}

sf::Sprite& BulletEntity::getSprite()
{
    return mSprite;
}

shared_ptr<Entity> BulletEntity::getParent()
{
    return mParent.lock();
}

bool BulletEntity::isActive() const
{
    return mActive;
}
