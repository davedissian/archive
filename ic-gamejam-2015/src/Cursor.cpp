#include "Common.h"
#include "Cursor.h"
#include "Turret.h"
#include "Map.h"

Cursor::Cursor()
{
    mRect.setSize(Vec2f(TILE_SIZE, TILE_SIZE));
    mRect.setFillColor(sf::Color(0, 255, 0, 32));
    mRect.setOutlineColor(sf::Color(0, 255, 0, 127));
    mRect.setOutlineThickness(-SCALE);

    mRange.setFillColor(sf::Color(64, 128, 64, 32));
}

void Cursor::ClearTurret()
{
    mHeldTurret.reset();
}

void Cursor::SetTurret(int id)
{
    mHeldTurret = make_shared<Turret>(id, true);
    mRange.setRadius(mHeldTurret->GetRange());
    mRange.setOrigin(mRange.getRadius(), mRange.getRadius());
}

void Cursor::PlaceTurret()
{
    if (mHeldTurret && mCanPlace && gGame->GetBalance() >= mHeldTurret->GetWorth())
    {
        gGame->AlterBalance(-mHeldTurret->GetWorth());
        mHeldTurret->Place();
        gGame->AddTurret(mHeldTurret);
        SetTurret(mHeldTurret->GetType());
        mHeldTurret->SetPosition(mPosition);
        SetPosition(mPosition);
    }
}

void Cursor::SetPosition(const Vec2i& position)
{
    mPosition = position;

    if (mHeldTurret)
        mHeldTurret->SetPosition(mPosition);

    // Align to grid
    Vec2i newPos;
    newPos.x = mPosition.x * TILE_SIZE;
    newPos.y = mPosition.y * TILE_SIZE;
    mRect.setPosition(newPos.x, newPos.y);
    mRange.setPosition(mRect.getPosition() + Vec2f(TILE_OFFSET, TILE_OFFSET));

    // Is something here already?
    mCanPlace = true;
    if (gGame->GetMap()->TrackAt(mPosition))
    {
        mCanPlace = false;
    }
    else
    {
        for (auto& t : gGame->Turrets())
        {
            Vec2i tp = t->GetPosition();
            if (tp.x == position.x && tp.y == position.y)
            {
                mCanPlace = false;
                break;
            }
        }
    }

    // Select colour based on placement
    if (mCanPlace)
    {
        mRect.setFillColor(sf::Color(0, 255, 0, 32));
        mRect.setOutlineColor(sf::Color(0, 255, 0, 127));
    }
    else
    {
        mRect.setFillColor(sf::Color(255, 0, 0, 32));
        mRect.setOutlineColor(sf::Color(255, 0, 0, 127));
    }
}

void Cursor::Draw()
{
    if (mHeldTurret)
    {
        mHeldTurret->Draw();
        gGame->Draw(mRange);
    }

    gGame->Draw(mRect);
}
