#include "Common.h"
#include "Turret.h"
#include "Train.h"
#include "Bullet.h"

TurretInfo info[] =
{
    {"turret", "T1", 1000, 3.0f * TILE_SIZE, 10.0f, 0.5f, Bullet::Ball, 10.0f,
        "This turret is simple and reliable. Fires bullets at a slow but steady speed."},
    {"turret", "T1R", 1500, 4.0f * TILE_SIZE, 15.0f, 0.1f, Bullet::Ball, 1.0f,
        "A more upgraded version of the T1 which has greater range and a much higher "
        "firing speed"},
    {"turret3", "SAM", 1200, 8.0f * TILE_SIZE, 2.0f, 1.0f, Bullet::Missile, 10.0f,
        "The SAM missile launcher is the first gen missile launcher, which is cheap "
        "and has a huge range, trading off for damage effectiveness."},
    {"turret2", "COIL", 4000, 6.0f * TILE_SIZE, 0.0f, 0.6f, Bullet::Lightning, 20.0f,
        "The coil is the product of cutting edge research in the realm of train "
        "destruction. Fires a bolt of energy which is designed to dish out significant "
        "damage."}
};

Turret::Turret(int type, bool beingPlaced)
    : mBeingPlaced(beingPlaced),
      mType(type),
      mInfo(info[type]),
      mTimeSinceLastShot(0.0f)
{
    mBase.setScale(SCALE, SCALE);
    mBase.setTexture(gGame->LoadTexture("media/" + mInfo.image + "-base.png"));
    mBase.setOrigin(TILE_OFFSET / SCALE, TILE_OFFSET / SCALE);
    mGun.setScale(SCALE, SCALE);
    mGun.setTexture(gGame->LoadTexture("media/" + mInfo.image + ".png"));
    mGun.setOrigin(TILE_OFFSET / SCALE, TILE_OFFSET / SCALE);
}

Turret::~Turret()
{
}

void Turret::SetPosition(const Vec2i& p)
{
    mPosition = p;
    SetPixelPosition(Vec2f(p.x, p.y) * (float)TILE_SIZE + Vec2f(TILE_OFFSET, TILE_OFFSET));
}

void Turret::SetPixelPosition(const Vec2f& p)
{
    mBase.setPosition(p);
    mGun.setPosition(p);
}

bool Turret::Update(float dt)
{
    // Search for the nearest train within range
    for (auto& t : gGame->Trains())
    {
        Vec2f diff = t->GetSprite().getPosition() - mBase.getPosition();
        float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist < mInfo.range)
        {
            // Calculate angle to train
            float angle = atan2(diff.y, diff.x) * 180.0f / M_PI + 90.0f;
            mGun.setRotation(angle);
            
            // Fire a bullet if the timeout has elapsed
            if (mTimeSinceLastShot > mInfo.cooldown)
            {
                mTimeSinceLastShot = 0.0f;
                Vec2f velocity = diff * mInfo.speed * (float)TILE_SIZE / dist;
                gGame->AddBullet(make_shared<Bullet>(
                            mBase.getPosition(), velocity, t, mInfo.type, mInfo.damage));
            }
            else
            {
                mTimeSinceLastShot += dt;
            }
            break;
        }
    }

    return true;
}

void Turret::Draw()
{
    gGame->Draw(mBase);
    gGame->Draw(mGun);
}

