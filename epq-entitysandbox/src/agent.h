// EPQ Project
#pragma once

// Forward declarations
class Item;
class ItemManager;

enum AgentState
{
	AS_IDLE,
	AS_CURIOUS,
	AS_FLEE
};

class Agent
{
public:
	Agent( ItemManager* itemMgr, const sf::Vector2f& position );
	~Agent();

	void update( float dt );

	void setPosition( const sf::Vector2f& position );
	sf::Vector2f getPosition() const;
	float angleToPosition( const sf::Vector2f& position, float factor ) const;

	void changeState( int newState );

	void setWaypoint( const sf::Vector2f& position );
	void clearWaypoint();

	void onNewItem( Item* item );
	void onDestroyItem( Item* item );
	void changeAttributes( Item* item, float pain, float temperature, float adrenaline, float enjoyment );

	string debugOutput() const;

private:
	ItemManager* mItemMgr;

	sf::ConvexShape mShape;
	float mHeading;
	int mState;

	// Motion
	float mMinDistance;
	float mMoveSpeed;
	float mTargetHeading;
	pair<bool, sf::Vector2f> mWaypoint;

	// Attributes
	float mPain;
	float mTemperature;
	float mAdrenaline;
	float mEnjoyment;

	// Items
	int mStartTime;
	Item* mTargetItem;
	list<Item*> mItemsToExplore;
	list<int> mItemsToAvoid;
	map<int, float> mExperienceIndexes;

};
