#pragma once

class Object;

class Trigger
{
public:
    Trigger(const Vec2& min, const Vec2& max);
    ~Trigger();

    void update(float dt);
    void render(sf::RenderWindow& window);

    bool testObject(shared_ptr<Object> object);

private:
    Vec2 min_;
    Vec2 max_;

    sf::RectangleShape shape_;
};
