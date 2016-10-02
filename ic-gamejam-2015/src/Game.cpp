#include "Common.h"
#include "Cursor.h"
#include "Map.h"
#include "Train.h"
#include "Turret.h"
#include "Bullet.h"
#include "Explosion.h"

Game* gGame;

// Source: https://gist.github.com/LiquidHelium/7858095
sf::String wrapText(sf::String str, unsigned width, const sf::Font &font, unsigned charSize, bool bold = false){
  unsigned currentOffset = 0;
  bool firstWord = true;
  std::size_t wordBegining = 0;
 
  for (std::size_t pos(0); pos < str.getSize(); ++pos) {
    auto currentChar = str[pos];
    if (currentChar == '\n'){
      currentOffset = 0;
      firstWord = true;
      continue;
    } else if (currentChar == ' ') {
      wordBegining = pos;
      firstWord = false;
    }
 
    auto glyph = font.getGlyph(currentChar, charSize, bold);
    currentOffset += glyph.advance;
 
    if (!firstWord && currentOffset > width) {
      pos = wordBegining;
      str[pos] = '\n';
      firstWord = true;
      currentOffset = 0;
    }
  }
 
  return str;
}

Game::Game()
    : mWindow(sf::VideoMode(SCREEN_X, SCREEN_Y), "IC Game Jam 2015"),
      mState(STATE_MENU),
      mDifficulty(0),
      mGUIOpen(false),
      mGUISlide(0.0f),
      mBalance(5000),
      mRemaining(0),
      mEscaped(0),
      mSpawnCounter(0),
      mTimeSinceLastSpawn(0.0f)
{
    gGame = this;

    mWindow.setVerticalSyncEnabled(true);
    mFont.loadFromFile("media/font.ttf");
    
    // Title
    mTitle.setFont(mFont);
    mTitle.setString("DESTROY ALL TRAINS!");
    mTitle.setCharacterSize(40);
    mTitle.setStyle(sf::Text::Bold);
    mTitle.setOrigin(mTitle.getLocalBounds().width / 2, mTitle.getLocalBounds().height / 2);
    mTitle.setPosition(SCREEN_X / 2, SCREEN_Y / 6);

    mCaption.setFont(mFont);
    mCaption.setString("Entry to ICGJ15 by David Avedissian");
    mCaption.setCharacterSize(14);
    mCaption.setOrigin(mCaption.getLocalBounds().width, -20);
    mCaption.setPosition(mTitle.getPosition() + mTitle.getOrigin());

    mAim.setFont(mFont);
    mAim.setCharacterSize(18);
    mAim.setString(
            "The aim of this game is to build a defense against the various\n"
            "trains which are moving through the map. Destroying trains earns\n"
            "you some decent money which you can then spend on more over the\n"
            "top train destroying weaponry.\n\n\n"
            "Press 1 for Level 1\n"
            "Press 2 for Level 2\n");
    mAim.setOrigin(mAim.getLocalBounds().width / 2, 0);
    mAim.setPosition(SCREEN_X / 2, SCREEN_Y / 2);

    // Map data
    mCursor = make_shared<Cursor>();

    // GUI
    mBuildMode.setFont(mFont);
    mBuildMode.setString("BUILD MODE\nPress <Enter> to start the round\n");
    mBuildMode.setCharacterSize(16);

    mBalanceDisplay.setFont(mFont);
    mBalanceDisplay.setCharacterSize(16);
    mBalanceDisplay.setPosition(Vec2f(SCREEN_X, SCREEN_Y));

    mTurretSelectorBox.setSize(Vec2f(TILE_SIZE * 2, TILE_SIZE * MAP_Y));
    mTurretSelectorBox.setFillColor(sf::Color(64, 64, 64, 127));
    mTurretSelectorCursor.setSize(Vec2f(TILE_SIZE, TILE_SIZE));
    mTurretSelectorCursor.setFillColor(sf::Color(0, 0, 0, 0));
    mTurretSelectorCursor.setOutlineColor(sf::Color(0, 255, 0, 127));
    mTurretSelectorCursor.setOutlineThickness(-SCALE);
    mTurretSelectorCursor.setOrigin(TILE_OFFSET, TILE_OFFSET);
    for (int i = 0; i < 4; i++)
    {
        auto turret = make_shared<Turret>(i, false);
        turret->SetPixelPosition(Vec2f(TILE_SIZE, TILE_SIZE * (i + 1)));
        mPossibleTurrets.push_back(move(turret));
    }
    mSelectedTurret = mPossibleTurrets.end();

    mTooltipBox.setSize(Vec2f(TILE_SIZE * 5.0f, TILE_SIZE * 3.0f));
    mTooltipBox.setOutlineColor(sf::Color(128, 128, 128));
    mTooltipBox.setFillColor(sf::Color(64, 64, 64));
    mTooltip.setFont(mFont);
    mTooltip.setCharacterSize(16);
}

int Game::Run()
{
    while (mWindow.isOpen())
    {
        sf::Event event;
        while (mWindow.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                mWindow.close();
                break;

            case sf::Event::KeyPressed:
                switch (mState)
                {
                case STATE_MENU:
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        mMap = make_shared<Map>("media/map1.data");
                        mState = STATE_BUILD;
                    }
                    else if (event.key.code == sf::Keyboard::Num2)
                    {
                        mMap = make_shared<Map>("media/map2.data");
                        mState = STATE_BUILD;
                    }
                    break;

                case STATE_BUILD:
                    if (event.key.code == sf::Keyboard::Escape)
                        mCursor->ClearTurret();
                    if (event.key.code == sf::Keyboard::Return)
                        StartRound();
                    break;

                default:
                    break;
                }
                break;

            case sf::Event::MouseMoved:
                switch (mState)
                {
                case STATE_BUILD:
                case STATE_GAME:
                    if (mGUIOpen)
                    {
                        if (event.mouseMove.x > mTurretSelectorBox.getSize().x)
                            SetGUIVisible(false);

                        // See if the cursor is over a turret
                        bool found = false;
                        for (auto i = mPossibleTurrets.begin(); i != mPossibleTurrets.end(); i++)
                        {
                            if ((*i)->GetBaseSprite().getGlobalBounds().contains(
                                    event.mouseMove.x, event.mouseMove.y))
                            {
                                // Position the selection box
                                mTurretSelectorCursor.setPosition((*i)->GetPixelPosition());
                                mSelectedTurret = i;

                                // Update the tooltip
                                mTooltipBox.setPosition((*i)->GetPixelPosition() +
                                        Vec2f(TILE_OFFSET, -TILE_OFFSET));
                                mTooltip.setPosition(mTooltipBox.getPosition() + Vec2f(10.0f, 10.0f));

                                // Set tooltip text
                                stringstream tooltip;
                                tooltip << "Name:   " << (*i)->GetName() << endl;
                                tooltip << "Price:  $" << (*i)->GetWorth() << endl;
                                tooltip << endl;
                                tooltip << (*i)->GetDesc();

                                // Ensure the string fits within the box
                                mTooltip.setString(wrapText(
                                            tooltip.str(),
                                            mTooltipBox.getSize().x - 20.0f,
                                            mFont, mTooltip.getCharacterSize(), false));

                                // Mark as found
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                            mSelectedTurret = mPossibleTurrets.end();
                    }
                    else
                    {
                        if (event.mouseMove.x < 32)
                             SetGUIVisible(true);

                        mCursor->SetPosition(Vec2i(
                            event.mouseMove.x / TILE_SIZE,
                            event.mouseMove.y / TILE_SIZE));
                    }
                    break;

                default:
                    break;
                }
                break;

            case sf::Event::MouseButtonPressed:
                switch (mState)
                {
                case STATE_MENU:
                    break;

                case STATE_BUILD:
                    if (mGUIOpen)
                    {
                        if (mSelectedTurret != mPossibleTurrets.end())
                        {
                            mCursor->SetTurret((*mSelectedTurret)->GetType());
                            SetGUIVisible(false);
                        }
                    }
                    else
                    {
                        if (event.mouseButton.button == sf::Mouse::Left)
                            mCursor->PlaceTurret();
                    }
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        // Update all objects
        Update(1.f / 60.f);
        DrawAll();
    }

    return 0;
}

void Game::Draw(sf::Drawable& drawable)
{
    mWindow.draw(drawable);
}

sf::Texture& Game::LoadTexture(const string& filename)
{
    auto it = mTextures.find(filename);
    if (it == mTextures.end())
    {
        sf::Texture texture;
        texture.loadFromFile(filename);
        mTextures.insert(make_pair(filename, move(texture)));
        return mTextures[filename];
    }
    else
    {
        return it->second;
    }
}

void Game::SpawnExplosion(const Vec2f& p, bool large)
{
    mExplosions.emplace_back(new Explosion(p, large));
}

void Game::Update(float dt)
{
    if (mMap)
        mMap->Update(dt);

    // Update trains
    for (auto i = mTrains.begin(); i != mTrains.end(); i++)
    {
        if (!(*i)->Update(dt))
        {
            SpawnExplosion((*i)->GetSprite().getPosition(), true);
            mRemaining--;
            i = mTrains.erase(i);
            if (i == mTrains.end())
                break;
        }
    }

    // Update bullets
    for (auto i = mBullets.begin(); i != mBullets.end(); i++)
    {
        if (!(*i)->Update(dt))
        {
            i = mBullets.erase(i);
            if (i == mBullets.end())
                break;
        }
    }

    // Update explosions
    for (auto i = mExplosions.begin(); i != mExplosions.end(); i++)
    {
        if (!(*i)->Update(dt))
        {
            i = mExplosions.erase(i);
            if (i == mExplosions.end())
                break;
        }
    }

    if (mState == STATE_GAME)
        DirectorTick(dt);

    if (mState == STATE_GAME && mRemaining == 0)
        mState = STATE_BUILD;

    // Update turrets
    for (auto& t : mTurrets)
        t->Update(dt);

    // Update UI
    if (mState == STATE_BUILD)
        SetGUISlide(lerp(mGUISlide, mGUIOpen ? 1.0f : 0.0f, 10.0f * dt));
}

void Game::DrawAll()
{
    mWindow.clear(sf::Color(0, 30, 5));
   
    if (mMap)
        mMap->Draw();
    for (auto& t : mTrains)
        t->Draw();
    for (auto& t : mTurrets)
        t->Draw();
    for (auto& b : mBullets)
        b->Draw();
    for (auto& e : mExplosions)
        e->Draw();

    switch (mState)
    {
    case STATE_MENU:
        {
            // Draw menu UI
            Draw(mTitle);
            Draw(mCaption);
            Draw(mAim);
        }
        break;

    case STATE_BUILD:
    case STATE_GAME:
        {
            // Draw persistent UI
            stringstream balanceStr;
            balanceStr << "Balance:    $" << mBalance << endl;
            balanceStr << "Difficulty: " << mDifficulty << endl;
            balanceStr << "Remaining:  " << mRemaining << " trains" << endl;
            balanceStr << "Escapees:   " << mEscaped << " trains";
            mBalanceDisplay.setString(balanceStr.str());
            auto bounds = mBalanceDisplay.getLocalBounds(); 
            mBalanceDisplay.setOrigin(Vec2f(bounds.left + bounds.width, bounds.top + bounds.height));
            Draw(mBalanceDisplay);

            if (mState == STATE_GAME)
            {
                for (auto& t : mTrains)
                    t->DrawHealth();
                break;
            }
            else
            {
                Draw(mBuildMode);
            }

            // Draw UI stuff
            if (!mGUIOpen)
                mCursor->Draw();
            
            // Draw the turret select UI
            if (!floatEq(mGUISlide, 0.0f, 0.1f))
            {
                Draw(mTurretSelectorBox);
                for (auto& t : mPossibleTurrets)
                    t->Draw();

                if (mSelectedTurret != mPossibleTurrets.end())
                {
                    Draw(mTurretSelectorCursor);
                    Draw(mTooltipBox);
                    Draw(mTooltip);
                }
            }
        }
        break;

    default:
        break;

    }
    
    mWindow.display();
}

void Game::SetGUISlide(float slide)
{
    mTurretSelectorBox.setPosition(-mTurretSelectorBox.getSize().x * (1.0f - slide), 0.0f);
    for (int i = 0; i < mPossibleTurrets.size(); i++)
    {
        mPossibleTurrets[i]->SetPixelPosition(
                mTurretSelectorBox.getPosition() + Vec2f(TILE_SIZE, TILE_SIZE * (i + 1)));
    }

    mGUISlide = slide;
}

void Game::SetGUIVisible(bool visible)
{
    mGUIOpen = visible;
}

void Game::StartRound()
{
    mState = STATE_GAME;
    mTrainSize = mDifficulty / 2 + 1;
    mRemaining = mSpawnCounter = 10 + mDifficulty * 5;
    mTimeSinceLastSpawn = 0;
}

void Game::DirectorTick(float dt)
{
    mTimeSinceLastSpawn += dt;
    if (mTimeSinceLastSpawn > 0.8f * mTrainSize)
    {
        mTimeSinceLastSpawn = 0.0f;
        if (mSpawnCounter > 0)
        {
            mSpawnCounter--;
            DispatchTrain();
        }
    }
}

void Game::DispatchTrain()
{
    mTrains.emplace_back(new Train(
                mMap->GetWaypoint(),
                mTrainSize,
                2.0f,
                100.0f * (1 + mDifficulty),
                800 + mDifficulty * 200 * mTrainSize));
}

