#pragma once

class Object;

// Global game state
class Game
{
public:
    uint width, height;

    // SFML
    sf::RenderWindow window;
    sf::View view;
    sf::Font font;

    // Box2D
    b2World world;

    // Constructor
    Game() : world(b2Vec2(0.0f, 100.0f))
    {
    }
};
