// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "Entity.h"

Entity::Entity(const Vec2& position) : mPosition(position)
{
}

Entity::~Entity()
{
}

void Entity::update(float dt)
{
}

void Entity::render(sf::RenderWindow& window)
{
}

void Entity::setPosition(const Vec2& position)
{
    mPosition = position;
}

const Vec2& Entity::getPosition() const
{
    return mPosition;
}
