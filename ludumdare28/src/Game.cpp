// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "Game.h"
#include "Renderer.h"
#include "EntityManager.h"
#include "BulletManager.h"

float Game::now = 0.0f;
int Game::score = 0;

enum GameState
{
    GS_NONE,
    GS_MENU,
    GS_GAME
};

int lastScore = 0;
int highScore = 0;

sf::Font font;
GameState currentState = GS_NONE;

#define PLAYER_ALIVE (EntityManager::inst().getPlayer() != shared_ptr<PlayerEntity>())

// MENU
sf::Text title;
sf::Text caption;
sf::Text prompt1;
sf::Text prompt2;
sf::Text prompt3;
sf::Text highScorePrompt;

// GAME
sf::Text gamePrompt;
sf::Text controls;
vector<sf::Text> gameOverPrompt;
uint shipID;
weak_ptr<EnemyEntity> boss;
int initialRowSize = 4;
int rowSize = initialRowSize;
int enemyCounter = 1;
float lastScoreTime = 0.0f;
float spawnInterval = 5.0f;
float lastSpawnTime = 0.0f;
float gameOverTimer = 0.0f;

void switchState(GameState state)
{
    // Leave previous state
    switch (currentState)
    {
    case GS_MENU:
    {
    }
    break;

    case GS_GAME:
    {
        EntityManager::inst().clearAllEntities();
    }
    break;

    default:
        break;
    }

    // Enter next state
    switch (state)
    {
    case GS_MENU:
    {
        sf::FloatRect rect;
        title.setString("A [___] GAME");
        title.setFont(font);
        title.setCharacterSize(64);
        rect = title.getLocalBounds();
        title.setColor(sf::Color(255, 255, 255, 255));
        title.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        title.setPosition(Vec2(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 4));

        caption.setString("Written by David Avedissian");
        caption.setFont(font);
        caption.setCharacterSize(15);
        caption.setColor(sf::Color(255, 255, 255, 255));
        caption.setPosition(Vec2(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 4 + 30));

        prompt1.setString("Press <1> to play as Ship 1");
        prompt1.setFont(font);
        prompt1.setCharacterSize(30);
        rect = prompt1.getLocalBounds();
        prompt1.setColor(sf::Color(255, 255, 255, 255));
        prompt1.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        prompt1.setPosition(Vec2(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2 + 30));

        prompt2.setString("Press <2> to play as Ship 2");
        prompt2.setFont(font);
        prompt2.setCharacterSize(30);
        rect = prompt2.getLocalBounds();
        prompt2.setColor(sf::Color(255, 255, 255, 255));
        prompt2.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        prompt2.setPosition(Vec2(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2 + 60));

        prompt3.setString("Press <Esc> to exit");
        prompt3.setFont(font);
        prompt3.setCharacterSize(30);
        rect = prompt3.getLocalBounds();
        prompt3.setColor(sf::Color(255, 255, 255, 255));
        prompt3.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        prompt3.setPosition(Vec2(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2 + 90));
    }
    break;

    case GS_GAME:
    {
        Game::score = 0;
        rowSize = initialRowSize;
        enemyCounter = 1;
        lastScoreTime = 0.0f;
        spawnInterval = 5.0f;
        lastSpawnTime = 0.0f;
        gameOverTimer = 0.0f;

        gamePrompt.setFont(font);
        gamePrompt.setCharacterSize(14);
        gamePrompt.setPosition(Vec2(0.0f, 0.0f));

        controls.setString("<wasd> for movement - <shift> for bullet-time - <space> for shooting");
        controls.setFont(font);
        controls.setCharacterSize(14);
        controls.setPosition(Vec2(0.0f, Game::SCREEN_HEIGHT - 14.0f));

        // Create player
        EntityManager::inst().createPlayer(
            Vec2((float)Game::SCREEN_WIDTH / 2, (float)Game::SCREEN_HEIGHT * 2.0f / 3.0f), shipID);
    }
    break;

    default:
        break;
    }

    currentState = state;
}

void updateState(float dt)
{
    switch (currentState)
    {
    case GS_MENU:
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            shipID = 1;
            switchState(GS_GAME);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            shipID = 2;
            switchState(GS_GAME);
        }
    }
    break;

    case GS_GAME:
    {
        stringstream str;
        str << "Score: " << Game::score << endl;
        if (PLAYER_ALIVE)
        {
            str << "Player Health: " << EntityManager::inst().getPlayer()->getHealth() << endl;
            str << "Bullet Time: "
                << EntityManager::inst().getPlayer()->getRemainingBulletTime() * 100.0f << "%"
                << endl;
        }
        else
        {
            str << "Player dead" << endl;
        }
        str << "Difficulty: " << rowSize << endl;
        gamePrompt.setString(str.str());

        // Advance game over
        if (!PLAYER_ALIVE)
        {
            if ((Game::now - gameOverTimer) > 5.0f)
                switchState(GS_MENU);
        }
        else
        {
            // Update score
            if ((Game::now - lastScoreTime) > 1.0f)
            {
                lastScoreTime = Game::now;
                Game::score += 100;

                // Truncate last 2 zeros
                int truncatedScore = (int)floor((float)Game::score / 100.0f) * 100;

                // Increase difficulty every 30 seconds from 1 minute onwards
                if (truncatedScore > 5999)
                {
                    int difficultyThreshold = 3000;
                    if (rowSize < 10)
                    {
                        if (truncatedScore % difficultyThreshold == 0)
                            rowSize++;
                    }
                    else if (spawnInterval > 1.0f)
                    {
                        if (truncatedScore % difficultyThreshold == 0)
                            spawnInterval -= 0.1f;
                    }
                }

                // Spawn the boss every 3 minutes
                if (truncatedScore % (3 * 60 * 100) == 0 &&
                    boss.lock() == shared_ptr<EnemyEntity>())
                    boss = EntityManager::inst().createEnemy(Vec2(Game::SCREEN_WIDTH / 2, -64.0f),
                                                             "boss1");
            }
            if (boss.lock() == shared_ptr<EnemyEntity>())
            {
                if ((Game::now - lastSpawnTime) > spawnInterval / Game::getTimeRate())
                {
                    lastSpawnTime = Game::now;

                    // Pick next enemy
                    string enemy = "enemy" + to_string(enemyCounter);
                    enemyCounter++;
                    if (enemyCounter > 3)
                        enemyCounter = 1;

                    // Spawn a row of them
                    for (int x = 1; x < (rowSize + 1); x++)
                        EntityManager::inst().createEnemy(
                            Vec2((float)x * Game::SCREEN_WIDTH / (rowSize + 1), -16.0f), enemy);
                }
            }
        }
    }
    break;

    default:
        break;
    }
}

void renderState(sf::RenderWindow& window)
{
    switch (currentState)
    {
    case GS_MENU:
    {
        window.draw(title);
        window.draw(caption);
        window.draw(prompt1);
        window.draw(prompt2);
        window.draw(prompt3);
        window.draw(highScorePrompt);
    }
    break;

    case GS_GAME:
    {
        window.draw(gamePrompt);
        window.draw(controls);
        if (!PLAYER_ALIVE)
        {
            for (auto i = gameOverPrompt.begin(); i != gameOverPrompt.end(); ++i)
                window.draw(*i);
        }
    }
    break;

    default:
        break;
    }
}

int Game::run()
{
    new ResourceCache;
    new Renderer(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, false);
    new EntityManager;
    new BulletManager;

    // Read from the high score file
    {
        ifstream highScoreFile("highScore.txt");
        highScoreFile >> highScore;
    }

    if (!font.loadFromFile("media/pf_arma_five.ttf"))
        throw std::runtime_error("Can't load font");
    switchState(GS_MENU);

    // Run the program as long as the window is open
    sf::Clock clock;
    while (Renderer::inst().getWindow().isOpen())
    {
        // Get time elapsed since last frame
        float time = clock.getElapsedTime().asSeconds();
        if (time < EPSILON)
            time = 1.0f / 60.0f;
        clock.restart();

        // Run a frame
        float dt = time * getTimeRate();
        now += dt;
        updateState(dt);
        Renderer::inst().processEvents();
        EntityManager::inst().updateAll(dt);
        Renderer::inst().beginRender();
        Renderer::inst().render(dt);
        renderState(Renderer::inst().getWindow());
        Renderer::inst().display();
    }

    // Tear down
    BulletManager::release();
    EntityManager::release();
    Renderer::release();
    ResourceCache::release();

    // Write to the high score file
    {
        ofstream highScoreFile("highScore.txt");
        highScoreFile << highScore;
    }

    return EXIT_SUCCESS;
}

void Game::onDeath()
{
    lastScore = Game::score;
    highScore = Game::score;
    gameOverTimer = Game::now;

    vector<string> lines;
    lines.push_back("You were destroyed!");
    lines.push_back("Game over...");
    lines.push_back("Score: " + to_string(lastScore));
    lines.push_back("");
    lines.push_back("Returning to the main menu");

    for (int i = 0; i < lines.size(); ++i)
    {
        sf::Text text(lines[i], font, 40);
        sf::FloatRect rect = text.getLocalBounds();
        text.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
        text.setPosition(Vec2(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 3 + 40 * i));
        gameOverPrompt.push_back(text);
    }
}

float Game::getTimeRate()
{
    if (EntityManager::inst().getPlayer() != shared_ptr<PlayerEntity>())
        return EntityManager::inst().getPlayer()->inBulletTime() ? 0.25f : 1.0f;
    else
        return 1.0f;
}
