#include "main.h"
#include "game.h"
#include "object.h"

Object::Object(const Vec2& p, b2World& world, Shape shapeType, float param, float param2,
               bool dynamic)
    : isDynamic_(dynamic), pulseTime_(0.0f), isObjective_(false), world_(&world), body_(nullptr)
{
    // Set up Box2D
    b2BodyDef bodyDef;
    bodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position.Set(p.x, p.y);
    body_ = world.CreateBody(&bodyDef);

    // Set up the shape
    shared_ptr<b2Shape> shape;
    switch (shapeType)
    {
    case S_BOX:
    {
        auto rect = make_shared<sf::RectangleShape>();
        rect->setSize(Vec2(param, param));
        rect->setOrigin(Vec2(param * 0.5f, param * 0.5f));
        shape_ = rect;

        auto poly = make_shared<b2PolygonShape>();
        poly->SetAsBox(param * 0.5f, param * 0.5f);
        shape = poly;
        break;
    }

    case S_CIRCLE:
    {
        auto circle = make_shared<sf::CircleShape>();
        circle->setRadius(param * 0.5f);
        circle->setOrigin(Vec2(param * 0.5f, param * 0.5f));
        shape_ = circle;

        shape = make_shared<b2CircleShape>();
        shape->m_radius = param * 0.5f;
        break;
    }

    case S_RECT:
    {
        auto rect = make_shared<sf::RectangleShape>();
        rect->setSize(Vec2(param, param2));
        rect->setOrigin(Vec2(param * 0.5f, param2 * 0.5f));
        shape_ = rect;

        auto poly = make_shared<b2PolygonShape>();
        poly->SetAsBox(param * 0.5f, param2 * 0.5f);
        shape = poly;
        break;
    }

    default:
    {
        assert(false && "Need to specify a shape");
        break;
    }
    }

    // Set mass settings
    if (dynamic)
    {
        b2FixtureDef fixtureDef;
        fixtureDef.shape = shape.get();
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 0.1f;
        body_->CreateFixture(&fixtureDef);
    }
    else
    {
        body_->CreateFixture(shape.get(), 0.0f);
    }

    // Set shape properties
    shape_->setFillColor(sf::Color::Black);
}

Object::~Object()
{
    world_->DestroyBody(body_);
}

void Object::setAsObjective(bool objective)
{
    isObjective_ = objective;
}

void Object::update(float dt)
{
    // If this is an objective object, pulsate the colour
    if (isObjective_)
    {
        pulseTime_ += dt;
        sf::Color first(sf::Color::Black), second(32, 64, 128);

        // Lerp colour between first and second
        float t = sin(pulseTime_) * 0.5f + 0.5f;
        sf::Color result(lerp(first.r, second.r, t), lerp(first.g, second.g, t),
                         lerp(first.b, second.b, t));
        shape_->setFillColor(result);
    }
}

void Object::render(sf::RenderWindow& window)
{
    shape_->setPosition(getPosition());
    shape_->setRotation(body_->GetAngle() * (180.0f / b2_pi));
    window.draw(*shape_);
}

void Object::applyForce(const Vec2& force, float dt)
{
    body_->ApplyLinearImpulse(toBox(force), body_->GetWorldCenter(), true);
}

bool Object::isDynamic() const
{
    return isDynamic_;
}

bool Object::isObjective() const
{
    return isObjective_;
}

Vec2 Object::getPosition() const
{
    return fromBox(body_->GetPosition());
}