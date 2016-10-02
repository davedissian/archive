// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef Renderer_h__
#define Renderer_h__

#include "Singleton.h"

class Explosion
{
public:
    Explosion(const Vec2& position, float size);
    ~Explosion();

    bool update(float dt);
    void render(sf::RenderWindow& window);

private:
    sf::Texture* mTexture;
    sf::Sprite mSprite;

    float mProgress;
};

class Renderer : public Singleton<Renderer>
{
public:
    Renderer(uint width, uint height, bool fullscreen);
    ~Renderer();

    void createExplosion(const Vec2& position, float size);

    void processEvents();
    void beginRender();
    void render(float dt);
    void display();

    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow mWindow;

    list<shared_ptr<Explosion>> mExplosions;

    sf::Texture* mBackgroundTexture;
    sf::Sprite mBackground;
};

#endif    // Renderer_h__
