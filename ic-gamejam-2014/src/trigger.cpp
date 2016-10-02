#include "main.h"
#include "trigger.h"
#include "object.h"

Trigger::Trigger(const Vec2& min, const Vec2& max) : min_(min), max_(max)
{
    shape_.setFillColor(sf::Color(64, 255, 64, 64));
    shape_.setPosition(min);
    shape_.setSize(max - min);
}

Trigger::~Trigger()
{
}

void Trigger::update(float dt)
{
}

void Trigger::render(sf::RenderWindow& window)
{
    window.draw(shape_);
}

bool Trigger::testObject(shared_ptr<Object> object)
{
    Vec2 position = object->getPosition();
    return position.x > min_.x && position.x < max_.x && position.y > min_.y && position.y < max_.y;
}