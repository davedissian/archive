#pragma once

#include "entity.h"
#include "level.h"

class State
{
public:
    State(shared_ptr<Game> game) : game_(game)
    {
    }
    virtual ~State()
    {
    }

    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void renderHUD() = 0;

    static void switchState(shared_ptr<State> next);
    static shared_ptr<State> getCurrent();

private:
    static shared_ptr<State> currentState_;

protected:
    shared_ptr<Game> game_;
};

class MenuState : public State
{
public:
    MenuState(shared_ptr<Game> game);
    ~MenuState();

    void update(float dt) override;
    void render() override;
    void renderHUD() override;

private:
    sf::Text title_;
    sf::Text caption_;
};

typedef vector<shared_ptr<Level>> LevelList;

class GameState : public State
{
public:
    GameState(shared_ptr<Game> game);
    ~GameState();

    void update(float dt) override;
    void render() override;
    void renderHUD() override;

    void initLevel();

private:
    Entity entity_;

    LevelList levels_;
    uint currentLevel_;

    float possessBarSize_;
    sf::RectangleShape possessBar_;
};

class CreditsState : public State
{
public:
    CreditsState(shared_ptr<Game> game);
    ~CreditsState();

    void update(float dt) override;
    void render() override;
    void renderHUD() override;

private:
    sf::Text caption_;
};