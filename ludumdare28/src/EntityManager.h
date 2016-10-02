// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef EntityManager_h__
#define EntityManager_h__

#include "Singleton.h"
#include "Entity.h"
#include "PlayerEntity.h"
#include "EnemyEntity.h"
#include "EnemyPartEntity.h"
#include "BulletEntity.h"
#include "MissileWeaponEntity.h"
#include "LaserWeaponEntity.h"

class EntityManager : public Singleton<EntityManager>
{
public:
    EntityManager();
    ~EntityManager();

    shared_ptr<PlayerEntity> createPlayer(const Vec2& position, uint shipID);
    shared_ptr<EnemyEntity> createEnemy(const Vec2& position, const string& name);
    shared_ptr<EnemyPartEntity> createEnemyPart(EnemyPartDesc& partDesc,
                                                shared_ptr<EnemyEntity> parent);
    shared_ptr<BulletEntity> createBullet(bool friendly);
    shared_ptr<MissileWeaponEntity> createMissileWeapon(const Vec2& startingPosition,
                                                        weak_ptr<DamageableEntity> target);
    shared_ptr<LaserWeaponEntity> createLaserWeapon(const Vec2& startingPosition,
                                                    weak_ptr<DamageableEntity> target);
    shared_ptr<PlayerEntity> getPlayer() const;
    void destroyEntity(shared_ptr<Entity> ent);
    void clearAllEntities();

    void updateAll(float dt);

    vector<shared_ptr<Entity>>::iterator getEntitiesBegin();
    vector<shared_ptr<Entity>>::iterator getEntitiesEnd();
    vector<shared_ptr<BulletEntity>>::iterator getPlayerBulletsBegin();
    vector<shared_ptr<BulletEntity>>::iterator getPlayerBulletsEnd();
    vector<shared_ptr<BulletEntity>>::iterator getNonPlayerBulletsBegin();
    vector<shared_ptr<BulletEntity>>::iterator getNonPlayerBulletsEnd();
    vector<shared_ptr<SpecialWeaponEntity>>::iterator getSpecialWeaponsBegin();
    vector<shared_ptr<SpecialWeaponEntity>>::iterator getSpecialWeaponsEnd();

private:
    map<string, EnemyDesc> mEnemyDescs;

    weak_ptr<PlayerEntity> mPlayer;
    vector<shared_ptr<Entity>> mEntities;
    vector<shared_ptr<BulletEntity>>
        mPlayerBullets;    // bullets are stored seperately for collision detection
    vector<shared_ptr<BulletEntity>>
        mNonPlayerBullets;    // bullets are stored seperately for collision detection
    vector<shared_ptr<SpecialWeaponEntity>> mSpecialWeapons;
};

#endif    // EntityManager_h__
