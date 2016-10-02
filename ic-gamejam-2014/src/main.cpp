#include "main.h"
#include "object.h"
#include "states.h"

int main()
{
    shared_ptr<Game> game = make_shared<Game>();

    // Create the window
    game->width = 800;
    game->height = 600;
    game->window.create(sf::VideoMode(game->width, game->height),
                        "Posessed - A Physics-based Puzzle Game");

    // Set up the view
    game->view = game->window.getDefaultView();

    // Load the font
    if (!game->font.loadFromFile("font.ttf"))
    {
        cerr << "Unable to load font file" << endl;
        return EXIT_FAILURE;
    }

    // Frame rate text
    sf::Text framerate;
    framerate.setFont(game->font);
    framerate.setCharacterSize(14);
    framerate.setColor(sf::Color::White);

    // Enter the main state
    State::switchState(make_shared<MenuState>(game));

    // Main Loop
    sf::Clock clock;
    float dt = 1.0f / 60.0f;
    while (game->window.isOpen())
    {
        // Handle Events
        sf::Event event;
        while (game->window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                game->window.close();
        }

        // Simulate physics
        game->world.Step(dt, 8, 3);

        // Update objects
        State::getCurrent()->update(dt);

        // Draw scene
        game->window.clear(sf::Color(200, 200, 200));

        // Draw scene
        game->window.setView(game->view);
        {
            State::getCurrent()->render();
        }

        // Draw HUD
        game->window.setView(game->window.getDefaultView());
        {
            // Update framerate
            float fps = 1.0f / dt;
            framerate.setString("Frame Rate: " + std::to_string(fps));
            game->window.draw(framerate);
            State::getCurrent()->renderHUD();
        }

        // Display
        game->window.display();

        // Get frame time
        dt = clock.getElapsedTime().asSeconds();
        clock.restart();
    }

    return EXIT_SUCCESS;
}