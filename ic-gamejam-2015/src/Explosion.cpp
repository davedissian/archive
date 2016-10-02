#include "Common.h"
#include "Explosion.h"

Explosion::Explosion(const Vec2f& pixelPosition, bool large)
    : mTimeSinceLastFrame(0.0f),
      mFrame(0)
{
    auto& texture = gGame->LoadTexture(large ? "media/explosion.png" : "media/explosion-small.png");

    // Frame count == aspect ratio
    mSize = texture.getSize().y;
    mMaxFrames = texture.getSize().x / mSize;

    mSprite.setScale(SCALE, SCALE);
    mSprite.setTexture(texture);
    mSprite.setTextureRect(sf::IntRect(0, 0, mSize, mSize));
    mSprite.setOrigin(mSize / 2, mSize / 2);
    mSprite.setPosition(pixelPosition);
}

Explosion::~Explosion()
{
}

bool Explosion::Update(float dt)
{
    mTimeSinceLastFrame += dt;

    // 12 frames a second
    if (mTimeSinceLastFrame > (1.0f / 12.0f))
    {
        mTimeSinceLastFrame = 0;
        mFrame++;

        if (mFrame == mMaxFrames)
            return false;
            
        mSprite.setTextureRect(sf::IntRect(mFrame * mSize, 0, mSize, mSize));
    }

    return true;
}

void Explosion::Draw()
{
    gGame->Draw(mSprite);
}
