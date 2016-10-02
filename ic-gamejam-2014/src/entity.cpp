#include "main.h"
#include "game.h"
#include "entity.h"

Entity::Entity(const Vec2& p)
    : radius_(20.0f),
      position_(p),
      velocity_(0.0f, 0.0f),
      possessionMeter_(0.0f),
      possessionCooldown_(false)
{
    shape_.setRadius(radius_);
    shape_.setOrigin(Vec2(radius_, radius_));
    shape_.setFillColor(sf::Color(0, 64, 128, 128));
    shape_.setPosition(position_);
}

void Entity::update(ObjectList& objects, float dt)
{
    shared_ptr<Object> possessed = possessed_.lock();

    possessionTimer_ = max(0.0f, min(5.0f, possessionTimer_ + (possessed ? dt : -dt)));

    if (possessed)
    {
        // Add forces to the object
        float force = 400000.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            possessed->applyForce(Vec2(0.0f, -force), dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            possessed->applyForce(Vec2(0.0f, force), dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            possessed->applyForce(Vec2(-force, 0.0f), dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            possessed->applyForce(Vec2(force, 0.0f), dt);
        }

        // Disowning the entity
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || possessionTimer_ > (5.0f - EPSILON))
        {
            position_ = possessed->getPosition();
            velocity_ = Vec2(0.0f, 0.0f);
            possessed_.reset();
            possessionCooldown_ = true;
        }
    }
    else
    {
        float speed = 1500.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            velocity_.y -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            velocity_.y += speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            velocity_.x -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            velocity_.x += speed;
        }

        position_.x += velocity_.x * dt;
        position_.y += velocity_.y * dt;
        shape_.setPosition(position_);

        // Attenuate velocity
        float attenuation = 0.05f;
        velocity_.x = lerp(velocity_.x, 0.0f, attenuation, dt);
        velocity_.y = lerp(velocity_.y, 0.0f, attenuation, dt);

        // If space key is pressed, begin possessing
        float time = 0.25f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            possessionMeter_ += dt / time;
        }
        else
        {
            possessionMeter_ -= dt / time;
        }
        possessionMeter_ = max(0.0f, min(1.0f, possessionMeter_));
        float scale = sqrt(1.0f - possessionMeter_ * 0.95f);
        shape_.setScale(scale, scale);

        // If the posessionMeter reaches 0, stop cooling down
        if (possessionMeter_ < EPSILON)
        {
            possessionCooldown_ = false;
        }

        // If the possessionMeter reaches 1, take control if not cooling down
        if (possessionMeter_ > (1.0f - EPSILON) && !possessionCooldown_)
        {
            // Search for the object the entity is floating over
            for (auto o : objects)
            {
                Vec2 pos = o->getPosition();
                float distance = pow(pos.x - position_.x, 2) + pow(pos.y - position_.y, 2);
                if (distance < (radius_ * radius_))
                {
                    // Possess this entity
                    possessed_ = o;
                    break;
                }
            }
        }
    }
}

void Entity::render(sf::RenderWindow& window)
{
    shared_ptr<Object> possessed = possessed_.lock();
    if (possessed)
    {
        // Draw some particle effects
    }
    else
    {
        window.draw(shape_);
    }
}

Vec2 Entity::getPosition() const
{
    shared_ptr<Object> possessed = possessed_.lock();
    if (possessed)
    {
        return possessed->getPosition();
    }
    else
    {
        return position_;
    }
}

float Entity::getPossessTimer() const
{
    return 1.0f - possessionTimer_ / 5.0f;
}

weak_ptr<Object> Entity::getPossessedObject()
{
    return possessed_;
}
