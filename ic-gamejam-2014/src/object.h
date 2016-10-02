#pragma once

enum Shape
{
    S_BOX,
    S_CIRCLE,
    S_RECT
};

class Object
{
public:
    Object(const Vec2& p, b2World& world, Shape shapeType, float param, float param2 = 0.0f,
           bool dynamic = true);
    ~Object();

    void setAsObjective(bool objective);
    void update(float dt);
    void render(sf::RenderWindow& window);

    void applyForce(const Vec2& force, float dt);

    bool isDynamic() const;
    bool isObjective() const;
    Vec2 getPosition() const;

private:
    bool isDynamic_;
    shared_ptr<sf::Shape> shape_;

    float pulseTime_;
    bool isObjective_;

    // Box2D stuff
    b2World* world_;
    b2Body* body_;
};

typedef vector<shared_ptr<Object>> ObjectList;
