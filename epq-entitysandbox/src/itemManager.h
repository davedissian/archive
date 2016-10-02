// EPQ Project
#pragma once
#include "items.h"

class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	void setAgent( Agent* agent );

	void setSpawnPosition( sf::Vector2f spawnPosition );
	sf::Vector2f getSpawnPosition() const;

	void switchToNextItem();
	void switchToPreviousItem();
	int getSelectedItemID() const;
	void spawnItem();
	void destroyItem( Item* item );
	std::list<Item*>& getSpawnedItemsList();

	void update( float dt );

	static string getName( int itemID );

private:
	Agent* mAgent;

	sf::Vector2f mSpawnPosition;
	sf::RectangleShape mSpawnPositionMarkerX;
	sf::RectangleShape mSpawnPositionMarkerY;
	int mSelectedItemID;

	std::list<Item*> mSpawnedItems;
	std::list<Item*> mItemsToRemove;

};
