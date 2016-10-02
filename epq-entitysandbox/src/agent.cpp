// EPQ Project
#include "main.h"
#include "agent.h"
#include "itemManager.h"

Agent::Agent( ItemManager* itemMgr, const sf::Vector2f& position ) :
	mItemMgr( itemMgr ),
	mHeading( 0.0f ),
	mState( AS_IDLE ),
	mMinDistance( 2.0f ),
	mMoveSpeed( 0.0f ),
	mPain( 0.0f ),
	mTemperature( 30.0f ),
	mAdrenaline( 0.0f ),
	mEnjoyment( 0.0f ),
	mStartTime( 0 ),
	mTargetItem( nullptr )
{
	mShape.setPosition( position );
	mShape.setPointCount( 3 );
	mShape.setPoint( 0, sf::Vector2f( 0.0f, -10.0f ) );
	mShape.setPoint( 1, sf::Vector2f( 10.0f, 10.0f ) );
	mShape.setPoint( 2, sf::Vector2f( -10.0f, 10.0f ) );
	addDrawable( mShape );

	// Set the initial state
	changeState( AS_IDLE );
}

Agent::~Agent()
{
	removeDrawable( mShape );
}

void Agent::update( float dt )
{
	// Update based on state
	switch( mState )
	{
	case AS_IDLE:
		{
			if( mWaypoint.first == false )
			{
				// Push a random position as a waypoint
				sf::Vector2f randomPos( (float)( rand() % (int)SCREEN_WIDTH ), (float)( rand() % (int)SCREEN_HEIGHT ) );

				// Make sure we dont cross the path of a bad object
				bool positionUnfeasable = false;
				for( auto i = mItemsToAvoid.begin(); i != mItemsToAvoid.end(); ++i )
				{
					if( positionUnfeasable )
						break;

					// Check for all instances of that object
					for( auto j = mItemMgr->getSpawnedItemsList().begin(); j != mItemMgr->getSpawnedItemsList().end(); ++j )
						if( (*j)->getID() == *i )
							if( getDistance( getPosition(), randomPos, (*j)->getPosition() ) < 100.0f && getDistance( getPosition(), (*j)->getPosition() ) > 100.0f )
							{
								positionUnfeasable = true;
								break;
							}
				}

				// If the path is still feasable then follow it
				if( positionUnfeasable == false )
					setWaypoint( randomPos );
			}

			if( !mItemsToExplore.empty() )
			{
				changeState( AS_CURIOUS );
			}
		}
		break;

	case AS_CURIOUS:
		{
			// If we've reached the waypoint then the object will have had an effect on us and
			// we can either process the next item in the
			if( mWaypoint.first == false || mTargetItem == nullptr )
			{
				if( !mItemsToExplore.empty() )
				{
					// Set the next target item
					mTargetItem = mItemsToExplore.front();
					mItemsToExplore.pop_front();

					// Skip the item if it's in the avoid list
					for( auto i = mItemsToAvoid.begin(); i != mItemsToAvoid.end(); ++i )
						if( *i == mTargetItem->getID() )
						{
							mTargetItem = nullptr;
							break;
						}

					// Check if we havent skipped the item
					if( mTargetItem != nullptr )
					{
						// Is this item good or unknown according to our past experience?
						int id = mTargetItem->getID();
						if( mExperienceIndexes.find( id ) == mExperienceIndexes.end() )
							mExperienceIndexes[id] = 0.0f;
						else if( mExperienceIndexes[id] <= 0.0f )
							mMoveSpeed = 60.0f;
						else
							mMoveSpeed = 100.0f;

						// Set min distance
						mMinDistance = mTargetItem->getMinDistance();
						mStartTime = gTimer.getElapsedTime().asMilliseconds();
					}
				}
				else
				{
					clearWaypoint();
					changeState( AS_IDLE );
				}
			}

			// Update the waypoint to the target item
			if( mTargetItem != nullptr )
			{
				setWaypoint( mTargetItem->getPosition() );

				// If it has taken too much time then give up chasing after it
				int timeTaken = gTimer.getElapsedTime().asMilliseconds() - mStartTime;
				if( timeTaken > 10000 )
				{
					changeAttributes( mTargetItem, 0.0f, 30.0f, 0.0f, -1.0f );
					mTargetItem = nullptr;
				}
			}
		}
		break;

	case AS_FLEE:
		{
			// If we've arrived at the flee point - become idle again
			if( mWaypoint.first == false )
				changeState( AS_IDLE );
		}
		break;

	default:
		changeState( AS_IDLE );
		break;
	}

	// Attenuate attributes
	static const float attenuation = 0.001f;
	mPain = lerp( mPain, 0.0f, 0.001f );
	mTemperature = lerp( mTemperature, 30.0f, attenuation );
	mAdrenaline = lerp( mAdrenaline, 0.0f, attenuation );
	mEnjoyment = lerp( mEnjoyment, 0.0f, attenuation );

	// Move towards the current waypoint
	if( mWaypoint.first == true )
	{
		sf::Vector2f& currentWaypoint = mWaypoint.second;

		// Determine if the waypoint is less than a step away, if it is then jump straight to it, otherwise make that step
		float oneStep = mMoveSpeed * dt;
		float distance = max( getDistance( mShape.getPosition(), currentWaypoint ) - mMinDistance, 0.0f );
		if( oneStep > distance )
		{
			mHeading = angleToPosition( currentWaypoint, 0.0f );
			mShape.setPosition( currentWaypoint - sf::Vector2f( cos( mHeading * DEG2RAD ), sin( mHeading * DEG2RAD ) ) * mMinDistance );
			clearWaypoint();
		}
		else
		{
			mHeading = angleToPosition( currentWaypoint, 0.95f );
			mShape.move( sf::Vector2f( cos( mHeading * DEG2RAD ), sin( mHeading * DEG2RAD ) ) * oneStep );
		}
	}

	// Update the shape
	mShape.setRotation( mHeading + 90.0f );
}

void Agent::setPosition( const sf::Vector2f& position )
{
	mShape.setPosition( position );
}

sf::Vector2f Agent::getPosition() const
{
	return mShape.getPosition();
}

float Agent::angleToPosition( const sf::Vector2f& position, float factor ) const
{
	sf::Vector2f delta = position - getPosition();
	float deltaAngle = atan2( delta.y, delta.x );
	return atan2( sin( mHeading * DEG2RAD ) * factor + sin( deltaAngle ) * ( 1.0f - factor ),
		cos( mHeading * DEG2RAD ) * factor + cos( deltaAngle ) * ( 1.0f - factor ) ) * RAD2DEG;
}

void Agent::changeState( int newState )
{
	mState = newState;

	switch( newState )
	{
	case AS_IDLE:
		mMoveSpeed = 120.0f;
		mMinDistance = 2.0f;
		break;

	case AS_CURIOUS:
		break;

	case AS_FLEE:
		{
			mMoveSpeed = 150.0f;
			mMinDistance = 2.0f;

			// Calculate a flee point
			sf::Vector2f fleePt = mWaypoint.second - mShape.getPosition();
			normalise( fleePt );
			fleePt *= 400.0f;
			fleePt.x = clamp( fleePt.x, 0.0f, SCREEN_WIDTH );
			fleePt.y = clamp( fleePt.y, 0.0f, SCREEN_HEIGHT );

			// Set this as the waypoint
			setWaypoint( fleePt );	
		}
		break;

	default:
		changeState( AS_IDLE );
		break;
	}
}

void Agent::setWaypoint( const sf::Vector2f& position )
{
	mWaypoint.first = true;
	mWaypoint.second = position;
}

void Agent::clearWaypoint()
{
	mWaypoint.first = false;
	mWaypoint.second = sf::Vector2f( 0.0f, 0.0f );
}

void Agent::onNewItem( Item* item )
{
	mItemsToExplore.push_back( item );
}

void Agent::onDestroyItem( Item* item )
{
	mItemsToExplore.remove( item );
	if( mTargetItem == item )
		mTargetItem = nullptr;
}

void Agent::changeAttributes( Item* item, float pain, float temperature, float adrenaline, float enjoyment )
{
	int id = item->getID();

	// Affect the attributes
	mPain += pain;
	mTemperature += temperature - 30.0f;
	mAdrenaline += adrenaline;
	mEnjoyment += enjoyment;

	// Calculate experience index based on the changes of attributes
	// The greater the magnitude of the experience index, the less influential it is
	float experienceIndex = -pain * 10.0f - abs( 30.0f - temperature ) - adrenaline + enjoyment;
	mExperienceIndexes[id] += experienceIndex * clamp( 0.05f / mExperienceIndexes[id], 0.0f, 1.0f );

	// If the experience index is < -10, add it to the unwanted list
	if( mExperienceIndexes[id] < -10.0f )
		mItemsToAvoid.push_back( id );

	// If the change in experience index is very high and negative, flee from it
	if( experienceIndex < -100.0f )
		changeState( AS_FLEE );
}

string Agent::debugOutput() const
{
	stringstream out;

	// Current State
	out << "Current State: ";
	switch( mState )
	{
	case AS_IDLE:
		out << "Idle";
		break;

	case AS_CURIOUS:
		out << "Curious";
		break;

	case AS_FLEE:
		out << "Flee";
		break;

	default:
		break;
	}
	out << endl << endl;

	// Attributes
	out << "Pain: " << mPain << endl;
	out << "Temperature: " << mTemperature << endl;
	out << "Adrenaline: " << mAdrenaline << endl;
	out << "Enjoyment: " << mEnjoyment << endl;
	out << endl;

	// Experience indexes
	out << "Experience Indexes:" << endl;
	if( mExperienceIndexes.empty() )
		out << "None" << endl;
	for( auto i = mExperienceIndexes.begin(); i != mExperienceIndexes.end(); i++ )
		out << "* " << ItemManager::getName( ( *i ).first ) << " - " << ( *i ).second << endl;
	out << endl;

	return out.str();
}