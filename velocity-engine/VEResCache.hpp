/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

// Container size
#define SIZE size_t

// Resource
#include "VEResource.hpp"

VE_BEGIN

// Resource Cache
class ResCache
{
						ResCache( SIZE sizeInMB );
						~ResCache();

	// Type Definitions
	typedef std::pair<Resource*, char*> ResourceDataPair;
	typedef std::pair<std::string, ResourceDataPair> CacheEntry;
	typedef std::map<std::string, ResourceDataPair> CacheList;
	typedef std::list<Resource*> ResourceList;

public:
	// Instance
	static ResCache		*getInstance();

	// Resource Handling
	Resource			*load( const std::string &filename );
	bool				makeRoom( SIZE size );

	// Accessors
	SIZE				size();
	SIZE				allocated();
	CacheList			&getCacheList();
	ResourceList		&getLRU();

private:
	// Resource Handling
	void				freeOneResource();
	char				*allocate( SIZE size );
	void				update( Resource *resource );

	// Lists
	CacheList			mCache;
	ResourceList		mLRU;

	// Properties
	SIZE				mSize;
	SIZE				mAllocated;

};

VE_END
