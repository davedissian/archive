// EPQ Project
#include "main.h"
#include "agent.h"
#include "items.h"
#include "itemManager.h"

ItemManager::ItemManager() :
	mAgent( nullptr ),
	mSpawnPosition( 0.0f, 0.0f ),
	mSelectedItemID( 0 )
{
	// Create the spawn position marker
	mSpawnPositionMarkerX.setSize( sf::Vector2f( 11.0f, 1.0f ) );
	mSpawnPositionMarkerX.setOrigin( 6.0f, 1.0f );
	mSpawnPositionMarkerX.setPosition( mSpawnPosition );
	mSpawnPositionMarkerY.setSize( sf::Vector2f( 1.0f, 11.0f ) );
	mSpawnPositionMarkerY.setOrigin( 1.0f, 6.0f );
	mSpawnPositionMarkerY.setPosition( mSpawnPosition );
	addDrawable( mSpawnPositionMarkerX );
	addDrawable( mSpawnPositionMarkerY );
}

ItemManager::~ItemManager()
{
	removeDrawable( mSpawnPositionMarkerX );
	removeDrawable( mSpawnPositionMarkerY );
}

void ItemManager::setAgent( Agent* agent )
{
	mAgent = agent;
}

void ItemManager::setSpawnPosition( sf::Vector2f spawnPosition )
{
	mSpawnPosition = spawnPosition;
	mSpawnPositionMarkerX.setPosition( mSpawnPosition );
	mSpawnPositionMarkerY.setPosition( mSpawnPosition );
}

sf::Vector2f ItemManager::getSpawnPosition() const
{
	return mSpawnPosition;
}

void ItemManager::switchToNextItem()
{
	++mSelectedItemID;
	if( mSelectedItemID > ( ID_OBJECT_COUNT - 1 ) )
		mSelectedItemID = 0;
}

void ItemManager::switchToPreviousItem()
{
	--mSelectedItemID;
	if( mSelectedItemID < 0 )
		mSelectedItemID = ID_OBJECT_COUNT - 1;
}

int ItemManager::getSelectedItemID() const
{
	return mSelectedItemID;
}

void ItemManager::spawnItem()
{
	Item* newItem = nullptr;
	switch( mSelectedItemID )
	{
	case ID_MINE:
		newItem = new MineItem( mSpawnPosition, this, mAgent );
		break;

	case ID_BALLOON:
		newItem = new BalloonItem( mSpawnPosition, this, mAgent );
		break;

	case ID_LIGHT:
		newItem = new LightItem( mSpawnPosition, this, mAgent );
		break;

	case ID_BALL:
		newItem = new BallItem( mSpawnPosition, this, mAgent );
		break;

	default:
		break;
	}

	if( newItem )
	{
		mSpawnedItems.push_back( newItem );
		mAgent->onNewItem( newItem );
	}
}

void ItemManager::destroyItem( Item* item )
{
	mAgent->onDestroyItem( item );
	mItemsToRemove.push_back( item );
}

std::list<Item*>& ItemManager::getSpawnedItemsList()
{
	return mSpawnedItems;
}

void ItemManager::update( float dt )
{
	// Update all items
	for( auto i = mSpawnedItems.begin(); i != mSpawnedItems.end(); i++ )
		( *i )->update( dt );

	// Remove flagged items
	for( auto i = mItemsToRemove.begin(); i != mItemsToRemove.end(); i++ )
	{
		mSpawnedItems.remove( ( *i ) );
		delete (*i);
	}
	mItemsToRemove.clear();
}

string ItemManager::getName( int itemID )
{
	switch( itemID )
	{
	case ID_MINE:
		return "ID_MINE";

	case ID_BALLOON:
		return "ID_BALLOON";

	case ID_LIGHT:
		return "ID_LIGHT";

	case ID_BALL:
		return "ID_BALL";

	default:
		break;
	}
	
	return "ID_UNKNOWN";
}