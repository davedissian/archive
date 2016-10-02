// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef BulletEntity_h__
#define BulletEntity_h__

#include "Entity.h"

class BulletEntity : public Entity
{
public:
    BulletEntity();
    ~BulletEntity();

    void spawn(const Vec2& position, const Vec2& velocity, shared_ptr<Entity> parent,
               sf::Texture* texture);
    void despawn();

    virtual void update(float dt);
    virtual void render(sf::RenderWindow& window);
    virtual void onCollision(shared_ptr<Entity> other);
    virtual sf::Sprite& getSprite();

    shared_ptr<Entity> getParent();
    bool isActive() const;

private:
    Vec2 mVelocity;
    float mRotation;
    weak_ptr<Entity> mParent;

    bool mActive;

    sf::Texture* mTexture;
    sf::Sprite mSprite;
};

#endif    // BulletEntity_h__
