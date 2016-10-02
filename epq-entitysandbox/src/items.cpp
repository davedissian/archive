// EPQ Project
#include "main.h"
#include "agent.h"
#include "items.h"
#include "itemManager.h"

Item::Item( sf::Vector2f position, ItemManager* itemMgr, Agent* agent ) :
	mPosition( position ),
	mItemMgr( itemMgr ),
	mAgent( agent )
{
}

Item::~Item()
{
}

void Item::kill()
{
	mItemMgr->destroyItem( this );
}

sf::Vector2f Item::getPosition()
{
	return mPosition;
}

MineItem::MineItem( sf::Vector2f position, ItemManager* itemMgr, Agent* agent ) :
	Item( position, itemMgr, agent )
{
	mShape.setFillColor( sf::Color( 127, 127, 127 ) );
	mShape.setSize( sf::Vector2f( 10.0f, 10.0f ) );
	mShape.setOrigin( 5.0f, 5.0f );
	mShape.setPosition( position );
	mShape.setRotation( (float)( rand() % 360 ) );
	addDrawable( mShape );
}

MineItem::~MineItem()
{
	removeDrawable( mShape );
}

void MineItem::update( float dt )
{
	float distanceToAgent = getDistance( mPosition, mAgent->getPosition() );
	if( distanceToAgent < 20.0f )
	{
		mAgent->changeAttributes( this, 10.0f, 100.0f, 0.0f, -10.0f );
		kill();
	}
}

BalloonItem::BalloonItem( sf::Vector2f position, ItemManager* itemMgr, Agent* agent ) :
	Item( position, itemMgr, agent )
{
	mShape.setFillColor( sf::Color( 200, 0, 0 ) );
	mShape.setRadius( 10.0f );
	mShape.setOrigin( 10.0f, 10.0f );
	mShape.setPosition( position );
	addDrawable( mShape );
}

BalloonItem::~BalloonItem()
{
	removeDrawable( mShape );
}

void BalloonItem::update( float dt )
{
	// 1 in 500 chance of popping every frame (60fps = 3 in 25 chance per second)
	if( rand() % 2000 == 0 )
	{
		float distanceToAgent = getDistance( mPosition, mAgent->getPosition() );
		if( distanceToAgent < 100.0f )
			mAgent->changeAttributes( this, 0.0f, 30.0f, 20.0f, 5.0f );
		kill();
	}
}

LightItem::LightItem( sf::Vector2f position, ItemManager* itemMgr, Agent* agent ) :
	Item( position, itemMgr, agent )
{
	mShape.setRadius( 2.0f );
	mShape.setOrigin( 2.0f, 2.0f );
	mShape.setPosition( position );
	addDrawable( mShape );
}

LightItem::~LightItem()
{
	removeDrawable( mShape );
}

void LightItem::update( float dt )
{
	// Glow brighter with increasing distance
	sf::Vector2f delta = mAgent->getPosition() - mPosition;
	float distance = getLength( delta );
	if( distance < 200.0f )
	{
		float intensity = 12.0f - distance * 0.005f * 12.0f;
		mShape.setRadius( 2.0f + intensity );
		mShape.setOrigin( mShape.getRadius(), mShape.getRadius() );
		mAgent->changeAttributes( this, 0.0f, 30.0f, 0.0f, intensity * 0.5f );
	}
}

BallItem::BallItem( sf::Vector2f position, ItemManager* itemMgr, Agent* agent ) :
	Item( position, itemMgr, agent )
{
	mShape.setFillColor( sf::Color( 0, 255, 0 ) );
	mShape.setRadius( 7.5f );
	mShape.setOrigin( 7.5f, 7.5f );
	mShape.setPosition( position );
	addDrawable( mShape );
	
	mDirChangeTime = gTimer.getElapsedTime().asSeconds();
	mDirection = 0.0f;
}

BallItem::~BallItem()
{
	removeDrawable( mShape );
}

void BallItem::update( float dt )
{
	if( ( gTimer.getElapsedTime().asSeconds() - mDirChangeTime ) > 2.0f )
	{
		mDirection = (float)( rand() % 200 - 100 ) * 0.01f;
		mDirChangeTime = gTimer.getElapsedTime().asSeconds();
	}

	// Move away from the agent
	sf::Vector2f delta = mAgent->getPosition() - mPosition;
	float distance = getLength( delta );
	if( distance < 60.0f )
	{
		normalise( delta );
		delta += sf::Vector2f( -delta.y, delta.x ) * mDirection;
		normalise( delta );
		delta *= pow( 1.0f - distance / 60.0f, 0.5f ) * 175.0f * dt;
		mPosition -= delta;

		// Update shape
		mShape.setPosition( mPosition );
	}
}