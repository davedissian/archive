// EPQ Project
#pragma once

// Forward declarations
class Agent;
class ItemManager;

enum ItemID
{
	ID_MINE = 0,
	ID_BALLOON,
	ID_LIGHT,
	ID_BALL,
	ID_OBJECT_COUNT
};

class Item
{
public:
	Item( sf::Vector2f position, ItemManager* itemMgr, Agent* agent );
	virtual ~Item();

	void kill();

	virtual void update( float dt ) = 0;
	virtual int getID() const = 0;
	virtual float getMinDistance() const = 0;

	sf::Vector2f getPosition();

protected:
	sf::Vector2f mPosition;
	ItemManager* mItemMgr;
	Agent* mAgent;

};

class MineItem : public Item
{
public:
	MineItem( sf::Vector2f position, ItemManager* itemMgr, Agent* agent );
	virtual ~MineItem();

	virtual void update( float dt );
	virtual int getID() const { return ID_MINE; }
	virtual float getMinDistance() const { return 10.0f; }

private:
	sf::RectangleShape mShape;

};

class BalloonItem : public Item
{
public:
	BalloonItem( sf::Vector2f position, ItemManager* itemMgr, Agent* agent );
	virtual ~BalloonItem();

	virtual void update( float dt );
	virtual int getID() const { return ID_BALLOON; }
	virtual float getMinDistance() const { return 30.0f; }

private:
	sf::CircleShape mShape;

};

class LightItem : public Item
{
public:
	LightItem( sf::Vector2f position, ItemManager* itemMgr, Agent* agent );
	virtual ~LightItem();

	virtual void update( float dt );
	virtual int getID() const { return ID_LIGHT; }
	virtual float getMinDistance() const { return 30.0f; }

private:
	sf::CircleShape mShape;

};

class BallItem : public Item
{
public:
	BallItem( sf::Vector2f position, ItemManager* itemMgr, Agent* agent );
	virtual ~BallItem();

	virtual void update( float dt );
	virtual int getID() const { return ID_BALL; }
	virtual float getMinDistance() const { return 15.0f; }

private:
	sf::CircleShape mShape;
	float mDirChangeTime;
	float mDirection;

};