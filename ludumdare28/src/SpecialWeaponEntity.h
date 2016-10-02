// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef SpecialWeaponEntity_h__
#define SpecialWeaponEntity_h__

#include "Entity.h"
#include "DameageableEntity.h"

class SpecialWeaponEntity : public Entity
{
public:
    SpecialWeaponEntity(const Vec2& startingPosition, weak_ptr<DamageableEntity> target);
    virtual ~SpecialWeaponEntity();

    weak_ptr<DamageableEntity> getTarget() const;

protected:
    weak_ptr<DamageableEntity> mTarget;
};

#endif    // SpecialWeaponEntity_h__
