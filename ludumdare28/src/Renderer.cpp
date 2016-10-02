// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "Renderer.h"
#include "EntityManager.h"

#define WIDTH 4
#define HEIGHT 5

Explosion::Explosion(const Vec2& position, float size) : mProgress(0.0f)
{
    mTexture = ResourceCache::inst().getTexture("explosion.png");

    Vec2 texSize((float)mTexture->getSize().x / WIDTH, (float)mTexture->getSize().y / HEIGHT);
    mSprite.setTexture(*mTexture);
    mSprite.setPosition(position);
    mSprite.setOrigin(texSize * 0.5f);
    mSprite.setScale(Vec2(size / (texSize.x * 0.5f), size / (texSize.y * 0.5f)));
}

Explosion::~Explosion()
{
}

bool Explosion::update(float dt)
{
    mProgress = step(mProgress, 1.0f, dt);

    if (mProgress == 1.0f)
        return false;

    // Update texture
    int currentFrame = (int)floor(mProgress * 20.0f);
    int x = currentFrame % WIDTH;
    int y = (int)floor((float)currentFrame / WIDTH);
    sf::Vector2<int> sub(mTexture->getSize().x / WIDTH, mTexture->getSize().y / HEIGHT);
    mSprite.setTextureRect(sf::IntRect(sf::Vector2<int>(x * sub.x, y * sub.y), sub));

    return true;
}

void Explosion::render(sf::RenderWindow& window)
{
    window.draw(mSprite);
}

Renderer::Renderer(uint width, uint height, bool fullscreen)
    : mWindow(sf::VideoMode(width, height), "LD28", fullscreen ? sf::Style::Fullscreen : 7U)
{
    // Set window settings
    mWindow.setVerticalSyncEnabled(true);

    // Load background
    mBackgroundTexture = ResourceCache::inst().getTexture("background.jpg");
    mBackground.setTexture(*mBackgroundTexture);
}

Renderer::~Renderer()
{
}

void Renderer::createExplosion(const Vec2& position, float size)
{
    shared_ptr<Explosion> exp(new Explosion(position, size));
    mExplosions.push_back(exp);
}

void Renderer::processEvents()
{
    // Check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
                mWindow.close();
            break;

        case sf::Event::Closed:
            mWindow.close();
            break;

        default:
            break;
        }
    }
}

void Renderer::beginRender()
{
    // mWindow.clear(sf::Color::Black);
}

void Renderer::render(float dt)
{
    // Draw background
    mWindow.draw(mBackground);

    // Draw entities
    for (auto i = EntityManager::inst().getEntitiesBegin();
         i != EntityManager::inst().getEntitiesEnd(); ++i)
        (*i)->render(mWindow);
    for (auto i = EntityManager::inst().getPlayerBulletsBegin();
         i != EntityManager::inst().getPlayerBulletsEnd(); ++i)
        (*i)->render(mWindow);
    for (auto i = EntityManager::inst().getNonPlayerBulletsBegin();
         i != EntityManager::inst().getNonPlayerBulletsEnd(); ++i)
        (*i)->render(mWindow);
    for (auto i = EntityManager::inst().getSpecialWeaponsBegin();
         i != EntityManager::inst().getSpecialWeaponsEnd(); ++i)
        (*i)->render(mWindow);

    if (EntityManager::inst().getPlayer() != shared_ptr<PlayerEntity>())
        EntityManager::inst().getPlayer()->_drawLocks(mWindow);

    // Update effects
    for (auto i = mExplosions.begin(); i != mExplosions.end();)
    {
        if (!(*i)->update(dt))
        {
            i = mExplosions.erase(i);
        }
        else
        {
            (*i)->render(mWindow);
            ++i;
        }
    }
}

void Renderer::display()
{
    mWindow.display();
}

sf::RenderWindow& Renderer::getWindow()
{
    return mWindow;
}
