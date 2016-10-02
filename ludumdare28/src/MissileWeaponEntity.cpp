#include "Common.h"
#include "MissileWeaponEntity.h"

MissileWeaponEntity::MissileWeaponEntity(const Vec2& startingPosition,
                                         weak_ptr<DamageableEntity> target)
    : SpecialWeaponEntity(startingPosition, target)
{
    mTexture = ResourceCache::inst().getTexture("missile.png");
    Vec2 textureSize((float)mTexture->getSize().x, (float)mTexture->getSize().y);
    mSprite.setTexture(*mTexture);
    mSprite.setOrigin(textureSize * 0.5f);
    mSprite.setRotation(180.0f);
}

MissileWeaponEntity::~MissileWeaponEntity()
{
}

void MissileWeaponEntity::update(float dt)
{
    float currentAngle = mSprite.getRotation();

    // Rotate towards target using lerp
    float speed = 600.0f;
    shared_ptr<DamageableEntity> target = mTarget.lock();
    if (target != shared_ptr<DamageableEntity>())
    {
        Vec2 targetDirection = target->getSprite().getPosition() - mPosition;
        float targetAngle = atan2(targetDirection.y, targetDirection.x) * RAD_TO_DEG + 90.0f;
        if (targetAngle > currentAngle)
        {
            while (targetAngle - currentAngle > 180.0f)
                targetAngle -= 360.0f;
        }
        else
        {
            while (targetAngle - currentAngle < -180.0f)
                targetAngle += 360.0f;
        }
        float swayFactor = 1.0f + sin(random(0.0f, TWO_PI));
        mSprite.setRotation(step(currentAngle, targetAngle, 500.0f * swayFactor * dt));
        speed = 600.0f / (1.0f + abs(targetAngle - currentAngle) / 180.0f);
    }

    // Move
    Vec2 direction(sin(currentAngle * DEG_TO_RAD), -cos(currentAngle * DEG_TO_RAD));
    mPosition += direction * speed * dt;
}

void MissileWeaponEntity::render(sf::RenderWindow& window)
{
    mSprite.setPosition(mPosition);
    window.draw(mSprite);
}

void MissileWeaponEntity::onCollision(shared_ptr<Entity> other)
{
}

sf::Sprite& MissileWeaponEntity::getSprite()
{
    return mSprite;
}
