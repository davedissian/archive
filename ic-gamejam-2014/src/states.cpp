#include "main.h"
#include "states.h"
#include "object.h"

shared_ptr<State> State::currentState_ = nullptr;

void State::switchState(shared_ptr<State> next)
{
    currentState_ = next;
}

shared_ptr<State> State::getCurrent()
{
    return currentState_;
}

MenuState::MenuState(shared_ptr<Game> game) : State(game)
{
    title_.setFont(game->font);
    title_.setCharacterSize(95);
    title_.setColor(sf::Color::White);
    title_.setString("Posessed");

    caption_.setFont(game->font);
    caption_.setCharacterSize(20);
    caption_.setColor(sf::Color::White);
    caption_.setString("Press <enter> to begin");

    float offset = title_.getLocalBounds().width * 0.5f;
    title_.setPosition(game->width * 0.5f - offset, game->height * 0.5f - 100.0f);
    caption_.setPosition(game->width * 0.5f - offset, game->height * 0.5f);
}

MenuState::~MenuState()
{
}

void MenuState::update(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        State::switchState(make_shared<GameState>(game_));
    }
}

void MenuState::render()
{
}

void MenuState::renderHUD()
{
    game_->window.draw(title_);
    game_->window.draw(caption_);
}

GameState::GameState(shared_ptr<Game> game)
    : State(game),
      entity_(Vec2(game->width * 0.5f, game->height * 0.5f)),
      currentLevel_(0),
      possessBarSize_(5.0f)
{
    // Add some objects
    std::default_random_engine random;
    std::uniform_real_distribution<float> posX(0.0f, game_->width);
    std::uniform_real_distribution<float> posY(0.0f, game_->height);

    // Create the possession rect
    possessBar_.setPosition(Vec2(0.0f, game_->height - possessBarSize_));
    possessBar_.setFillColor(sf::Color(64, 64, 255, 64));

    // Initialise the first level
    initLevel();
}

GameState::~GameState()
{
}

void GameState::update(float dt)
{
    // If the level has been won, advance to the next
    if ((*levels_.begin())->hasWon())
    {
        levels_.clear();
        initLevel();
        if (levels_.size() == 0)
        {
            State::switchState(make_shared<CreditsState>(game_));
        }
    }

    // Lerp between the entity position and camera position
    float nextX = entity_.getPosition().x;
    nextX = lerp(game_->view.getCenter().x, nextX, 0.025f, dt);
    game_->view.setCenter(nextX, game_->height * 0.5f);

    // Update Entity
    entity_.update((*levels_.begin())->getObjects(), dt);

    // Update current level
    (*levels_.begin())->update(entity_, dt);
}

void GameState::render()
{
    (*levels_.begin())->render(game_->window);
    entity_.render(game_->window);
}

void GameState::renderHUD()
{
    (*levels_.begin())->renderHUD(game_->window);

    possessBar_.setSize(Vec2(game_->width * entity_.getPossessTimer(), possessBarSize_));
    game_->window.draw(possessBar_);
}

void GameState::initLevel()
{
    switch (currentLevel_)
    {
    case 0:
    {
        shared_ptr<Level> l = make_shared<Level>(game_);

        float padding = 100.0f, thickness = 10.0f;
        Vec2 begin(padding, padding);
        Vec2 end(game_->width - padding, game_->height - padding);

        l->addBoxedRoom(begin, end, thickness);
        l->addObjectiveObject(
            make_shared<Object>(Vec2(400.0f, 300.0f), game_->world, S_BOX, 40.0f));
        l->setTriggerRegion(begin + Vec2(thickness, thickness),
                            Vec2(begin.x + 100.0f, end.y - thickness));
        l->addAnnotation(begin + Vec2(120.0f, 20.0f), 16,
                         "Welcome to POSESSED. This game involves taking control\n"
                         "of physical props and moving the glowing 'objective\n"
                         "props' into the green region. Start by moving to the\n"
                         "prop and holding <space>, then press <enter> to drop\n"
                         "the prop.");

        levels_.push_back(l);
        break;
    }

    case 1:
    {
        shared_ptr<Level> l = make_shared<Level>(game_);

        float thickness = 10.0f;
        Vec2 begin(200.0f, 50.0f);
        Vec2 end(600.0f, 550.0f);

        l->addBoxedRoom(begin, end, thickness);

        for (int i = 0; i < 4; ++i)
            l->addObject(make_shared<Object>(Vec2(325.0f + (float)i * 50.0f, 200.0f), game_->world,
                                             S_BOX, 40.0f));

        for (int i = 0; i < 5; ++i)
            l->addObject(make_shared<Object>(Vec2(300.0f + (float)i * 50.0f, 300.0f), game_->world,
                                             S_BOX, 40.0f));

        l->addObjectiveObject(
            make_shared<Object>(Vec2(400.0f, 400.0f), game_->world, S_BOX, 40.0f));
        l->setTriggerRegion(begin + Vec2(thickness, thickness),
                            Vec2(end.x - thickness, begin.y + 200.0f));
        l->addAnnotation(begin + Vec2(thickness + 20.0f, 220.0f), 16,
                         "Here's a tip - create a stack!");

        levels_.push_back(l);
        break;
    }

    case 2:
    {
        shared_ptr<Level> l = make_shared<Level>(game_);

        float thickness = 10.0f;
        Vec2 begin(0.0f, 120.0f);
        Vec2 end(1000.0f, 480.0f);

        l->addBoxedRoom(begin, end, thickness);

        for (int i = 1; i < 9; ++i)
            l->addObject(make_shared<Object>(Vec2(400.0f, 120.0f + (float)i * 40.0f), game_->world,
                                             S_BOX, 40.0f));
        for (int i = 1; i < 9; ++i)
            l->addObject(make_shared<Object>(Vec2(420.0f, 120.0f + (float)i * 40.0f), game_->world,
                                             S_BOX, 40.0f));

        l->addObjectiveObject(
            make_shared<Object>(Vec2(200.0f, 300.0f), game_->world, S_BOX, 40.0f));
        l->setTriggerRegion(Vec2(end.x - 100.0f - thickness, begin.y + thickness),
                            Vec2(end.x - thickness, end.y - thickness));
        l->addAnnotation(begin + Vec2(thickness + 20.0f, thickness + 20.0f), 16,
                         "BREAK THE WALL DOWN!");

        levels_.push_back(l);
        break;
    }
    }

    currentLevel_++;
}

CreditsState::CreditsState(shared_ptr<Game> game) : State(game)
{
    caption_.setFont(game->font);
    caption_.setCharacterSize(20);
    caption_.setColor(sf::Color::White);
    caption_.setString("Press <space> to return to the main menu");

    float offset = caption_.getLocalBounds().width * 0.5f;
    caption_.setPosition(game->width * 0.5f - offset, game->height * 0.5f);
}

CreditsState::~CreditsState()
{
}

void CreditsState::update(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        State::switchState(make_shared<MenuState>(game_));
    }
}

void CreditsState::render()
{
}

void CreditsState::renderHUD()
{
    game_->window.draw(caption_);
}
