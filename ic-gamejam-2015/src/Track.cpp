#include "Common.h"
#include "Track.h"

Track::Track(Track::Direction dir, const Vec2i& position)
{
    stringstream texture;
    texture << "media/tracks.png";
    mSprite.setScale(SCALE, SCALE);
    mSprite.setTexture(gGame->LoadTexture(texture.str()));
    mSprite.setTextureRect(sf::IntRect((int)dir * TILE_SIZE / SCALE, 0, TILE_SIZE / SCALE, TILE_SIZE / SCALE));
    mSprite.setPosition(position.x * TILE_SIZE, position.y * TILE_SIZE);
}

Track::~Track()
{
}

void Track::Draw()
{
    gGame->Draw(mSprite);
}
