// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef Entity_h__
#define Entity_h__

class Entity
{
public:
    Entity(const Vec2& position);
    virtual ~Entity();

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void onCollision(shared_ptr<Entity> other) = 0;

    virtual sf::Sprite& getSprite() = 0;

    void setPosition(const Vec2& position);
    const Vec2& getPosition() const;

protected:
    Vec2 mPosition;
};

#endif    // Entity_h__
