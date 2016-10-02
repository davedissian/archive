// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "PlayerEntity.h"
#include "BulletManager.h"
#include "EntityManager.h"
#include "Renderer.h"

PlayerEntity::PlayerEntity(const Vec2& position, uint shipID)
    : DamageableEntity(position, 1000, false),
      mRemainingBulletTime(1.0f),
      mChargingBulletTime(false),
      mShipID(shipID)
{
    // Load textures
    mShipTexture = ResourceCache::inst().getTexture("player" + to_string(shipID) + ".png");
    mShieldTexture = ResourceCache::inst().getTexture("shield.png");
    mTargetTexture = ResourceCache::inst().getTexture("target.png");
    mLockOnTexture = ResourceCache::inst().getTexture("lock.png");
    mLockingOnTexture = ResourceCache::inst().getTexture("lock-progress.png");

    // Load gun points
    // TODO move this to JSON
    if (shipID == 1)
    {
        mGunPoints.push_back(Vec2(-10.0f, -16.0f));
        mGunPoints.push_back(Vec2(10.0f, -16.0f));
        mGunPoints.push_back(Vec2(-22.0f, -8.0f));
        mGunPoints.push_back(Vec2(22.0f, -8.0f));
    }
    else if (shipID == 2)
    {
        mGunPoints.push_back(Vec2(-20.0f, -20.0f));
        mGunPoints.push_back(Vec2(20.0f, -20.0f));

        mLaserPoints.push_back(Vec2(-26.5f, -9.5f));
        mLaserPoints.push_back(Vec2(26.5f, -9.5f));
        mLaserPoints.push_back(Vec2(-26.5f, 3.5f));
        mLaserPoints.push_back(Vec2(26.5f, 3.5f));
        mLaserPoints.push_back(Vec2(-26.5f, 16.5f));
        mLaserPoints.push_back(Vec2(26.5f, 16.5f));
    }

    // Set up ship sprite
    Vec2 shipTextureSize((float)mShipTexture->getSize().x, (float)mShipTexture->getSize().y);
    mShipSprite.setTexture(*mShipTexture);
    mShipSprite.setOrigin(shipTextureSize * 0.5f);

    // Set up shield sprites
    Vec2 shieldTextureSize((float)mShieldTexture->getSize().x, (float)mShieldTexture->getSize().y);
    mShieldSprite.setTexture(*mShieldTexture);
    mShieldSprite.setOrigin(shieldTextureSize * 0.5f);
    for (uint i = 0; i < 10; ++i)
    {
        Shield shield;
        shield.visibility = 0.0f;
        shield.rotation = 0.0f;
        mShields.push_back(shield);
    }

    // Set up target sprite
    Vec2 targetTextureSize((float)mTargetTexture->getSize().x, (float)mTargetTexture->getSize().y);
    mTargetSprite.setTexture(*mTargetTexture);
    mTargetSprite.setOrigin(targetTextureSize * 0.5f);

    // Set up lock sprites
    Vec2 lockOnTextureSize((float)mLockOnTexture->getSize().x, (float)mLockOnTexture->getSize().y);
    mLockOnSprite.setTexture(*mLockOnTexture);
    mLockOnSprite.setOrigin(lockOnTextureSize * 0.5f);
    Vec2 lockingOnTextureSize((float)mLockingOnTexture->getSize().x,
                              (float)mLockingOnTexture->getSize().y);
    mLockingOnSprite.setTexture(*mLockingOnTexture);
    mLockingOnSprite.setOrigin(lockingOnTextureSize * 0.5f);

    // Set up player 2 stuff
    if (shipID == 2)
    {
        mLaserGunTexture = ResourceCache::inst().getTexture("player2-laser.png");
        mLaserTexture = ResourceCache::inst().getTexture("laser.png");

        // Set up sprites
        Vec2 laserGunTextureSize((float)mLaserGunTexture->getSize().x,
                                 (float)mLaserGunTexture->getSize().y);
        mLaserGunSprite.setTexture(*mLaserGunTexture);
        mLaserGunSprite.setOrigin(laserGunTextureSize * 0.5f);
        Vec2 laserTextureSize((float)mLaserTexture->getSize().x * 0.5f,
                              (float)mLaserTexture->getSize().y);
        mLaserSprite.setTexture(*mLaserTexture);
        mLaserSprite.setOrigin(laserTextureSize);
    }

    // Create locks
    uint noLocks = shipID == 2 ? mLaserPoints.size() : 8;
    for (uint i = 0; i < noLocks; ++i)
    {
        Lock lock;
        lock.hasLock = false;
        lock.lockProgress = 0.0f;
        mLocks.push_back(lock);
    }
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::damage(const Vec2& direction, uint damageTaken)
{
    DamageableEntity::damage(direction, damageTaken);

    // Flare up shield
    _hitShield(direction);
}

float PlayerEntity::getRemainingBulletTime() const
{
    return mRemainingBulletTime;
}

bool PlayerEntity::inBulletTime() const
{
    return mBulletTime;
}

void PlayerEntity::_specialAttack()
{
    // Get entities
    vector<shared_ptr<DamageableEntity>> entities;
    for (uint i = 0; i < mLocks.size(); ++i)
    {
        if (mLocks[i].hasLock && mLocks[i].weapon.lock() == shared_ptr<SpecialWeaponEntity>())
        {
            shared_ptr<DamageableEntity> entity = mLocks[i].target.lock();
            if (entity != shared_ptr<DamageableEntity>())
            {
                // Do something depending on currently selected player
                // TODO: script this

                // Player 1
                // Fire a savlo of missiles from the back of the ship
                if (mShipID == 1)
                    mLocks[i].weapon = EntityManager::inst().createMissileWeapon(mPosition, entity);
                else if (mShipID == 2)
                    mLocks[i].weapon = EntityManager::inst().createLaserWeapon(
                        mPosition + mLaserPoints[i], entity);
            }
            mLocks[i].fired = true;
        }
    }
}

void PlayerEntity::_hitShield(const Vec2& direction)
{
    // Take the front shield
    list<Shield>::iterator front = mShields.begin();

    // Flare that shield up
    (*front).visibility = 1.0f;
    (*front).rotation = atan2(direction.y, direction.x) * RAD_TO_DEG + 90.0f;

    // Move that shield to the back and shuffle the remaining shield elements forward by 1
    list<Shield>::iterator newFront = front;
    advance(newFront, 1);
    rotate(front, newFront, mShields.end());
}

void PlayerEntity::_drawLocks(sf::RenderWindow& window)
{
    // Draw lock ons
    static const float maxScale = 10.0f;
    for (auto i = mLocks.begin(); i != mLocks.end(); ++i)
    {
        if ((*i).hasLock)
        {
            shared_ptr<DamageableEntity> entity = (*i).target.lock();
            if (entity)
            {
                if ((*i).lockProgress < 1.0f)
                {
                    mLockingOnSprite.setPosition(entity->getSprite().getPosition());
                    float scale = maxScale - (maxScale - 1) * (*i).lockProgress;
                    mLockingOnSprite.setScale(scale, scale);
                    mLockingOnSprite.setColor(
                        sf::Color(255, 255, 255, (sf::Uint8)(255.0f * (*i).lockProgress)));
                    window.draw(mLockingOnSprite);
                }
                else
                {
                    mLockOnSprite.setPosition(entity->getSprite().getPosition());
                    window.draw(mLockOnSprite);
                }
            }
        }
    }
}

void PlayerEntity::update(float dt)
{
    // Death
    if (mHealth <= 0)
    {
        EntityManager::inst().destroyEntity(shared_from_this());
        Renderer::inst().createExplosion(mPosition, (float)mShipTexture->getSize().x);
        Game::onDeath();
    }

    // Bullet time
    mBulletTime = !mChargingBulletTime && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    mRemainingBulletTime =
        step(mRemainingBulletTime, mBulletTime ? 0.0f : 1.0f, dt / (mBulletTime ? 5.0f : 10.0f));
    if (mRemainingBulletTime < EPSILON)
        mChargingBulletTime = true;
    if (mRemainingBulletTime == 1.0f && mChargingBulletTime)
        mChargingBulletTime = false;

    // Shooting
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (mGunTimer.getElapsedTime().asSeconds() > 0.1f / Game::getTimeRate())
        {
            for (auto i = mGunPoints.begin(); i != mGunPoints.end(); ++i)
                BulletManager::inst().spawn(mPosition + (*i), Vec2(0.0f, -500.0f),
                                            shared_from_this(), BT_PLAYER_COMMON, true);
            mGunTimer.restart();
        }
    }

    // Advance locks
    for (auto i = mLocks.begin(); i != mLocks.end(); ++i)
    {
        if ((*i).hasLock)
        {
            // Remove the lock if the target or weapon was destroyed after it was fired
            if ((*i).target.lock() == shared_ptr<DamageableEntity>() ||
                ((*i).fired && (*i).weapon.lock() == shared_ptr<SpecialWeaponEntity>()))
            {
                (*i).target = shared_ptr<DamageableEntity>();
                (*i).lockProgress = 0.0f;
                (*i).hasLock = false;
                (*i).fired = false;
                (*i).weapon = shared_ptr<SpecialWeaponEntity>();
            }
            else
            {
                (*i).lockProgress += 4.0f * dt;
                if ((*i).lockProgress > 1.0f)
                    (*i).lockProgress = 1.0f;
            }
        }
    }

    // Special attack
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        _specialAttack();

    // Update velocity
    static const float MAX_VELOCITY = 300.0f;
    static const float ACCELERATION = 2000.0f;
    Vec2 targetVelocity(0.0f, 0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        targetVelocity.y = -MAX_VELOCITY;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        targetVelocity.y = MAX_VELOCITY;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        targetVelocity.x = -MAX_VELOCITY;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        targetVelocity.x = MAX_VELOCITY;
    mVelocity.x = step(mVelocity.x, targetVelocity.x, ACCELERATION * dt);
    mVelocity.y = step(mVelocity.y, targetVelocity.y, ACCELERATION * dt);

    // Force from edge of screen
    Vec2 edgeForce(0.0f, 0.0f);
    float margin = mShipTexture->getSize().x * 0.5f;
    if (mPosition.x < margin)
        edgeForce.x += margin - mPosition.x;
    if (mPosition.x > Game::SCREEN_WIDTH - margin)
        edgeForce.x -= margin - (Game::SCREEN_WIDTH - mPosition.x);
    if (mPosition.y < margin)
        edgeForce.y += margin - mPosition.y;
    if (mPosition.y > Game::SCREEN_HEIGHT - margin)
        edgeForce.y -= margin - (Game::SCREEN_HEIGHT - mPosition.y);
    edgeForce *= ACCELERATION;

    // Update position
    mPosition += mVelocity * dt + 0.5f * edgeForce * dt * dt;    // s = ut + 1/2at^2

    // Fade out shields
    for (auto i = mShields.begin(); i != mShields.end(); ++i)
    {
        if ((*i).visibility > EPSILON)
        {
            (*i).visibility -= dt;
            if ((*i).visibility < 0.0f)
                (*i).visibility = 0.0f;
        }
    }

    // Check if the target touched anything
    float targetRadiusSq = ((float)(mTargetTexture->getSize().x * mTargetTexture->getSize().x) +
                            (float)(mTargetTexture->getSize().y * mTargetTexture->getSize().y)) *
                           0.25f;
    for (auto i = EntityManager::inst().getEntitiesBegin();
         i != EntityManager::inst().getEntitiesEnd(); ++i)
    {
        if (*i != shared_from_this())
        {
            shared_ptr<DamageableEntity> ent = dynamic_pointer_cast<DamageableEntity>(*i);
            if (ent != shared_ptr<DamageableEntity>())
            {
                if (ent->isLockable())
                {
                    // The entitys centre must be inside the target for a lock to register
                    Vec2 delta = mTargetSprite.getPosition() - ent->getSprite().getPosition();
                    float distanceSq = delta.x * delta.x + delta.y * delta.y;
                    if (distanceSq < targetRadiusSq)
                    {
                        // Make sure this entity hasn't already been locked onto
                        // TODO max number of locks = ceil(health / missile damage)
                        uint lockedCounter = 0;
                        bool currentlyLockingOn = false;
                        for (auto l = mLocks.begin(); l != mLocks.end(); ++l)
                        {
                            if ((*l).target.lock() == ent)
                            {
                                lockedCounter++;
                                if ((*l).lockProgress < 1.0f)
                                {
                                    currentlyLockingOn = true;
                                    break;
                                }
                            }
                        }

                        // Find a free lock
                        // TODO magic number
                        uint maxLocks = (uint)floor((float)ent->getHealth() / 100.0f) + 1;
                        if (lockedCounter < maxLocks && !currentlyLockingOn)
                        {
                            for (auto i = mLocks.begin(); i != mLocks.end(); ++i)
                            {
                                // Lock on!!
                                if (!(*i).hasLock)
                                {
                                    (*i).hasLock = true;
                                    (*i).lockProgress = 0.0f;
                                    (*i).target = ent;
                                    (*i).fired = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void PlayerEntity::render(sf::RenderWindow& window)
{
    mShipSprite.setPosition(mPosition);
    window.draw(mShipSprite);

    mTargetSprite.setPosition(mPosition + Vec2(0.0f, -250.0f));
    window.draw(mTargetSprite);

    // Draw laser guns
    if (mShipID == 2)
    {
        for (uint i = 0; i < mLocks.size(); ++i)
        {
            Vec2 gunPosition = mPosition + mLaserPoints[i];
            mLaserGunSprite.setPosition(gunPosition);

            // Point at target
            shared_ptr<DamageableEntity> target = mLocks[i].target.lock();
            if (target != shared_ptr<DamageableEntity>())
            {
                Vec2 targetPos = target->getSprite().getPosition();
                Vec2 direction = targetPos - gunPosition;
                float angle = atan2(direction.y, direction.x) * RAD_TO_DEG + 90.0f;
                mLaserGunSprite.setRotation(angle);
                window.draw(mLaserGunSprite);

                // Draw lazoorz
                if (mLocks[i].weapon.lock() != shared_ptr<SpecialWeaponEntity>())
                {
                    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
                    mLaserSprite.setRotation(angle);
                    mLaserSprite.setPosition(gunPosition);
                    mLaserSprite.setScale(Vec2(1.0f, distance / (float)mLaserTexture->getSize().y));
                    window.draw(mLaserSprite);
                }
            }
            else
            {
                mLaserGunSprite.setRotation(0.0f);
                window.draw(mLaserGunSprite);
            }
        }
    }

    // Draw shields
    for (auto i = mShields.begin(); i != mShields.end(); ++i)
    {
        if ((*i).visibility > EPSILON)
        {
            mShieldSprite.setPosition(mPosition);
            mShieldSprite.setRotation((*i).rotation);
            mShieldSprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(255.0f * (*i).visibility)));
            window.draw(mShieldSprite);
        }
    }
}

void PlayerEntity::onCollision(shared_ptr<Entity> other)
{
    shared_ptr<BulletEntity> bullet = dynamic_pointer_cast<BulletEntity>(other);
    if (bullet != shared_ptr<BulletEntity>())
    {
        // Take damage
        damage(other->getPosition() - mPosition, 50);

        // Despawn the bullet
        bullet->despawn();
    }

    shared_ptr<EnemyEntity> enemy = dynamic_pointer_cast<EnemyEntity>(other);
    if (enemy != shared_ptr<EnemyEntity>())
    {
        if (other->getSprite().getTexture()->getSize().x > mShipTexture->getSize().x)
        {
            // Destroy the ship
            mHealth = 0.0f;
        }
        else
        {
            // Take lots of damage
            damage(other->getPosition() - mPosition, 200);

            // Destroy the other entity
            enemy->damage(other->getPosition() - mPosition, 999999);
        }
    }
}

sf::Sprite& PlayerEntity::getSprite()
{
    return mShipSprite;
}