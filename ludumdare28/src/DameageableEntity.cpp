// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "DameageableEntity.h"
#include "BulletEntity.h"
#include "EntityManager.h"

DamageableEntity::DamageableEntity(const Vec2& position, int health, bool lockable)
    : Entity(position), mHealth(health), mLockable(lockable)
{
}

DamageableEntity::~DamageableEntity()
{
}

int DamageableEntity::getHealth() const
{
    return mHealth;
}

bool DamageableEntity::isLockable() const
{
    return mLockable;
}

void DamageableEntity::damage(const Vec2& direction, uint damageTaken)
{
    mHealth -= damageTaken;
}

void DamageableEntity::onCollision(shared_ptr<Entity> other)
{
    shared_ptr<BulletEntity> bullet = dynamic_pointer_cast<BulletEntity>(other);
    if (bullet != shared_ptr<BulletEntity>())
    {
        // Take damage
        damage(other->getPosition() - mPosition, 10);

        // Despawn bullet
        bullet->despawn();
    }

    shared_ptr<SpecialWeaponEntity> special = dynamic_pointer_cast<SpecialWeaponEntity>(other);
    if (special != shared_ptr<SpecialWeaponEntity>())
    {
        // Take (lots) of damage
        // TODO magic number
        damage(other->getPosition() - mPosition, 100);
        EntityManager::inst().destroyEntity(special);
    }
}