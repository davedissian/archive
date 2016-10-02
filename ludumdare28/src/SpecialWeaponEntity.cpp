// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "SpecialWeaponEntity.h"

SpecialWeaponEntity::SpecialWeaponEntity(const Vec2& startingPosition,
                                         weak_ptr<DamageableEntity> target)
    : Entity(startingPosition), mTarget(target)
{
}

SpecialWeaponEntity::~SpecialWeaponEntity()
{
}

weak_ptr<DamageableEntity> SpecialWeaponEntity::getTarget() const
{
    return mTarget;
}