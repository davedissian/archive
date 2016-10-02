#include "main.h"
#include "object.h"
#include "level.h"
#include "trigger.h"

Level::Level(shared_ptr<Game> game) : game_(game), wonLevel_(false)
{
    countdownTimer_.pause();
    countdownText_.setPosition(Vec2(200.0f, 200.0f));
    countdownText_.setFont(game_->font);
    countdownText_.setCharacterSize(50);
    countdownText_.setColor(sf::Color::White);
}

Level::~Level()
{
}

void Level::setTriggerRegion(const Vec2& min, const Vec2& max)
{
    trigger_ = make_shared<Trigger>(min, max);
}

void Level::addObject(shared_ptr<Object> object)
{
    objects_.push_back(object);
}

void Level::addObjectiveObject(shared_ptr<Object> object)
{
    object->setAsObjective(true);
    objects_.push_back(object);
    objectiveObjects_.push_back(object);
}

void Level::addWall(const Vec2& begin, const Vec2& end)
{
    Vec2 size = end - begin;
    addObject(
        make_shared<Object>(begin + size * 0.5f, game_->world, S_RECT, size.x, size.y, false));
}

void Level::addBoxedRoom(const Vec2& begin, const Vec2& end, float thickness)
{
    addWall(begin, Vec2(end.x, begin.y + thickness));
    addWall(Vec2(begin.x, end.y - thickness), end);
    addWall(Vec2(begin.x, begin.y + thickness), Vec2(begin.x + thickness, end.y - thickness));
    addWall(Vec2(end.x - thickness, begin.y + thickness), Vec2(end.x, end.y - thickness));
}

void Level::addAnnotation(const Vec2& position, float size, std::string str)
{
    sf::Text text;
    text.setPosition(position);
    text.setString(str);
    text.setFont(game_->font);
    text.setCharacterSize(size);
    text.setColor(sf::Color::White);
    annotations_.push_back(text);
}

void Level::update(Entity& e, float dt)
{
    for (auto o : objects_)
    {
        o->update(dt);
    }

    bool allInside = true;
    for (auto o : objectiveObjects_)
    {
        // Test if any are not in the trigger zone
        if (!trigger_->testObject(o))
        {
            allInside = false;
            break;
        }
    }

    // If the entity is still possessing an object, don't count it
    if (e.getPossessedObject().lock())
    {
        allInside = false;
    }

    if (allInside)
    {
        // If all the blocks are inside, resume the countdown
        if (countdownTimer_.isPaused())
        {
            countdownTimer_.resume();
        }

        // Count
        uint seconds = 5 - (uint)countdownTimer_.getSeconds();

        // If seconds reaches 0, then we won the level
        if (seconds == 0)
        {
            wonLevel_ = true;
        }

        // Update the countdown
        countdownText_.setString("Countdown: " + std::to_string(seconds));
    }
    else
    {
        if (!countdownTimer_.isPaused())
        {
            countdownTimer_.pause();
            countdownTimer_.reset();
        }
    }
}

void Level::render(sf::RenderWindow& window)
{
    for (auto o : objects_)
    {
        o->render(window);
    }

    for (auto& t : annotations_)
    {
        window.draw(t);
    }

    if (trigger_)
    {
        trigger_->render(window);
    }
}

void Level::renderHUD(sf::RenderWindow& window)
{
    if (!countdownTimer_.isPaused())
    {
        window.draw(countdownText_);
    }
}

ObjectList& Level::getObjects()
{
    return objects_;
}

ObjectList& Level::getObjectiveObjects()
{
    return objectiveObjects_;
}

bool Level::hasWon() const
{
    return wonLevel_;
}