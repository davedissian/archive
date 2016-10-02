#include "Common.h"
#include "Bullet.h"
#include "Train.h"

Bullet::Bullet(const Vec2f& p, const Vec2f& v, weak_ptr<Train> target, Bullet::Type type, float damage)
    : mVelocity(v),
      mSpeed(10.0f * TILE_SIZE),
      mAcceleration(0.0f, 0.0f),
      mTarget(target),
      mType(type),
      mDamage(damage)
{
    if (type == Lightning)
    {
        int which = rand() % 10;
        mSprite.setTexture(gGame->LoadTexture("media/lightning.png"));

        Vec2i ls(102, 280);
        mSprite.setTextureRect(sf::IntRect(which * ls.x, 0.0f, ls.x, ls.y));
        mSprite.setOrigin(ls.x / 2, ls.y / 2);

        Vec2f tp = target.lock()->GetSprite().getPosition();
        Vec2f diff = tp - p;
        float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
        mSprite.setScale(120.0f / ls.x, dist / ls.y);
        mSprite.setPosition((p.x + tp.x) * 0.5f, (p.y + tp.y) * 0.5f);
        mSprite.setRotation(atan2(diff.y, diff.x) * 180.0f / M_PI + 90.0f);

        // Damage instantly
        target.lock()->Damage(mDamage);
    }
    else
    {
        if (mType == Missile)
        {
            mSprite.setScale(SCALE / 2, SCALE / 2);
            mSprite.setTexture(gGame->LoadTexture("media/missile.png"));
            mSprite.setOrigin(TILE_OFFSET / (SCALE / 4), TILE_OFFSET / (SCALE / 4));
        }
        else
        {
            mSprite.setScale(SCALE, SCALE);
            mSprite.setTexture(gGame->LoadTexture("media/bullet.png"));
            mSprite.setOrigin(TILE_OFFSET / (SCALE / 2), TILE_OFFSET / (SCALE / 2));
        }
        mSprite.setPosition(p.x, p.y);
    }
}

Bullet::~Bullet()
{
}

bool Bullet::Update(float dt)
{
    if (mType == Lightning)
    {
        // Fade away
        sf::Color newColour = mSprite.getColor();
        newColour.a = step((float)newColour.a, 0.0f, 512.0f * dt);
        mSprite.setColor(newColour);
    }
    else
    {
        if (mType == Missile)
        {
            // Point towards the target
            auto target = mTarget.lock();
            if (target)
            {
                mAcceleration = target->GetSprite().getPosition() - mSprite.getPosition();
                float length = len(mAcceleration);
                mAcceleration *= mSpeed / length;
                mSprite.setRotation(atan2(mVelocity.y, mVelocity.x) * 180.0f / M_PI + 90.0f);
            }
            mVelocity += mAcceleration * dt;
        }

        mSprite.setPosition(mSprite.getPosition() + mVelocity * dt);

        // Check whether this bullet intersects with a train
        for (auto& t : gGame->Trains())
        {
            if (t->GetBounds().contains(mSprite.getPosition()))
            {
                t->Damage(mDamage);
                if (mType == Missile)
                    gGame->SpawnExplosion(mSprite.getPosition(), false);
                return false;
            }
        }
    }
    return true;
}

void Bullet::Draw()
{
    gGame->Draw(mSprite);
}
