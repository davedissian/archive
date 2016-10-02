#include "Common.h"
#include "Game.h"

#include "Train.h"

Train::Train(Waypoint* waypoint, int size, float speed, float health, int worth)
    : mWaypoint(waypoint),
      mHealthBarVisibility(0.0f),
      mInitialHealth(health),
      mHealth(health),
      mSpeed(speed),
      mWorth(worth)
{
    
    mHealthBar.setOrigin(TILE_OFFSET, -TILE_OFFSET - 2);
  
    for (int i = 0; i < size; i++)
    {
        Vec2i start = waypoint->begin()->mPosition;
        Carrage carrage;
        carrage.sprite.setTexture(gGame->LoadTexture("media/train.png"));
        carrage.sprite.setPosition(start.x * TILE_SIZE + TILE_OFFSET, start.y * TILE_SIZE + TILE_OFFSET);
        carrage.sprite.setOrigin(TILE_OFFSET / SCALE, TILE_OFFSET / SCALE);
        carrage.sprite.setScale(SCALE, SCALE);
        carrage.waypoint = waypoint->begin() + 1;
        mCarrages.push_back(move(carrage));
    }
}

Train::~Train()
{
}

bool Train::Update(float dt)
{
    // If the entity has been marked as dead
    if (IsDead() || mHealth <= 0.0f)
    {
        gGame->AlterBalance(mWorth);
        return false;
    }

    Vec2f offset(TILE_OFFSET, TILE_OFFSET);
    float speed = TILE_SIZE * mSpeed * dt;

    {
        auto& mTarget = mCarrages[0].waypoint;
    
        // Step towards target
        Vec2f position = mCarrages[0].sprite.getPosition() - offset;
        Vec2i target = mTarget->mPosition * TILE_SIZE;
        position.x = step<float>(position.x, target.x, speed);
        position.y = step<float>(position.y, target.y, speed);

        // If it has reached the target, then move on to the next one
        if (floatEq<float>(position.x, target.x) && floatEq<float>(position.y, target.y))
        {
            mTarget++;
            if (mTarget == mWaypoint->end())
            {
                gGame->TrainEscaped();
                return false;
            }
        }

        mCarrages[0].sprite.setRotation(floatEq<float>(position.x, target.x) ? 0.0f : 90.0f);
        mCarrages[0].sprite.setPosition(position + offset);
        mHealthBar.setPosition(mCarrages[0].sprite.getPosition());
        mHealthBarVisibility = step(mHealthBarVisibility, 0.0f, dt);
    }

    return true;
}

void Train::Draw()
{
    for (auto& c : mCarrages)
        gGame->Draw(c.sprite);
}

void Train::DrawHealth()
{
    if (mHealthBarVisibility > numeric_limits<float>::epsilon())
    {
        float factor = mHealth / mInitialHealth;
        sf::Color c((int)lerp(255.0f, 0.0f, factor), (int)lerp(0.0f, 255.0f, factor), 0.0f);
        c.a = 255.0f * min(mHealthBarVisibility, 1.0f);
        mHealthBar.setFillColor(c);
        mHealthBar.setSize(Vec2f(TILE_SIZE * factor, 2.0f));
        gGame->Draw(mHealthBar);
    }

}

void Train::Damage(float amount)
{
    mHealthBarVisibility = 3.0f;
    mHealth -= amount;
}
