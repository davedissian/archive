#pragma once

#include "object.h"
#include "trigger.h"
#include "entity.h"

class Level
{
public:
    Level(shared_ptr<Game> game);
    ~Level();

    // Level Setup
    void setTriggerRegion(const Vec2& min, const Vec2& max);
    void addObject(shared_ptr<Object> object);
    void addObjectiveObject(shared_ptr<Object> object);

    void addWall(const Vec2& begin, const Vec2& end);
    void addBoxedRoom(const Vec2& begin, const Vec2& end, float thickness);

    void addAnnotation(const Vec2& position, float size, std::string text);

    // Update
    void update(Entity& e, float dt);
    void render(sf::RenderWindow& window);
    void renderHUD(sf::RenderWindow& window);

    // Accessors
    ObjectList& getObjects();
    ObjectList& getObjectiveObjects();
    bool hasWon() const;

private:
    shared_ptr<Game> game_;
    shared_ptr<Trigger> trigger_;

    vector<sf::Text> annotations_;

    Timer countdownTimer_;
    sf::Text countdownText_;

    ObjectList objects_;
    ObjectList objectiveObjects_;

    bool wonLevel_;
};
