#ifndef TURRET_H
#define TURRET_H

#include "Entity.h"
#include "Bullet.h"

class Train;

struct TurretInfo
{
    string image;
    string name;
    int worth;
    float range;

    // Bullet
    float speed;
    float cooldown;
    Bullet::Type type;
    float damage;

    // Description
    string description;
};

class Turret : public Entity
{
public:
    Turret(int type, bool beingPlaced);
    ~Turret();

    void Place() { mBeingPlaced = false; }
    void SetPosition(const Vec2i& p);
    Vec2i GetPosition() const { return mPosition; }
    void SetPixelPosition(const Vec2f& p);
    Vec2f GetPixelPosition() const { return mBase.getPosition(); }
    sf::Sprite& GetBaseSprite() { return mBase; }

    int GetType() const { return mType; }
    float GetRange() const { return mInfo.range; }
    int GetWorth() const { return mInfo.worth; }
    string GetName() const { return mInfo.name; }
    string GetDesc() const { return mInfo.description; }

    virtual void OnEvent(sf::Event&) {}
    virtual bool Update(float dt);
    virtual void Draw();

private:
    bool mBeingPlaced;

    int mType;
    TurretInfo mInfo;
    Vec2i mPosition;

    sf::Sprite mBase;
    sf::Sprite mGun;
    float mTimeSinceLastShot;
};

#endif /* TURRET_H */
