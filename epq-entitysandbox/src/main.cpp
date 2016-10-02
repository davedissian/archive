// EPQ Project
#include <Windows.h>
#include "main.h"
#include "agent.h"
#include "itemManager.h"

sf::Clock gTimer;
list<sf::Drawable*> gDrawables;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPTSTR, int )
{
	ItemManager* itemMgr;
	Agent* agent;

	// Initialise the random number generator to a some arbitrary seed
	srand( timeGetTime() );

	// Create the window
	sf::RenderWindow window;
	window.create( sf::VideoMode( (unsigned int)SCREEN_WIDTH, (unsigned int)SCREEN_HEIGHT ), "What are the building blocks of modelling intelligent behaviour in computer programs?" );
	window.setVerticalSyncEnabled( true );

	// Load the font
	sf::Font font;
	font.loadFromFile( "LiberationSans.ttf" );

	// Create some invisible text
	// This is needed as a workaround to fix a text rendering bug
	sf::Text _dummyText( "DUMMY", font, 16 );
	_dummyText.setPosition( -100.0f, -100.0f );
	addDrawable( _dummyText );

	// Create the debug text
	sf::Text debugText( "", font, 16 );
	addDrawable( debugText );

	// Set up the simulation
	// Bit of a catch-22 here...
	itemMgr = new ItemManager;
	agent = new Agent( itemMgr, sf::Vector2f( (float)( rand() % (int)SCREEN_WIDTH ), (float)( rand() % (int)SCREEN_WIDTH ) ) );
	itemMgr->setAgent( agent );

	// Run the main loop
	sf::Clock frameTimer;
	gTimer.restart();
	while( window.isOpen() )
	{
		// Process events
		sf::Event e;
		while( window.pollEvent( e ) )
		{
			// Window closed
			if( e.type == sf::Event::Closed )
				window.close();

			if( e.type == sf::Event::KeyPressed )
			{
				// Cycle between spawnable objects
				if( e.key.code == sf::Keyboard::Left )
					itemMgr->switchToPreviousItem();
				if( e.key.code == sf::Keyboard::Right )
					itemMgr->switchToNextItem();

				// Spawn the object at the position
				if( e.key.code == sf::Keyboard::Return )
					itemMgr->spawnItem();

				// Escape key pressed
				if( e.key.code == sf::Keyboard::Escape )
					window.close();
			}
		}

		// Update the simulation
		agent->update( frameTimer.getElapsedTime().asSeconds() );
		itemMgr->update( frameTimer.getElapsedTime().asSeconds() );
		frameTimer.restart();

		// Changing the spawn position
		if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
		{
			sf::Vector2i localPosition = sf::Mouse::getPosition( window );
			itemMgr->setSpawnPosition( sf::Vector2f( (float)localPosition.x, (float)localPosition.y ) );
		}

		// Update debug text
		stringstream debugStr;
		debugStr << "Selected Object: " << ItemManager::getName( itemMgr->getSelectedItemID() ) << endl;
		debugStr << endl;
		debugStr << agent->debugOutput();
		debugText.setString( debugStr.str() );
		
		// Render a frame
        window.clear( sf::Color( 0, 40, 100, 255 ) );
		for( auto i = gDrawables.begin(); i != gDrawables.end(); ++i )
			window.draw( **i ); // Double dereference, wtf?
		window.display();
	}

	window.close();
	return EXIT_SUCCESS;
}
