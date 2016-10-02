#pragma once

#include "object.h"

class Entity
{
public:
    Entity(const Vec2& p);

    void update(ObjectList& objects, float dt);
    void render(sf::RenderWindow& window);

    Vec2 getPosition() const;

    float getPossessTimer() const;
    weak_ptr<Object> getPossessedObject();

private:
    float radius_;
    Vec2 position_;
    Vec2 velocity_;
    sf::CircleShape shape_;

    float possessionMeter_;
    weak_ptr<Object> possessed_;
    float possessionTimer_;
    bool possessionCooldown_;
};