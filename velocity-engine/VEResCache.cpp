/*
	Velocity Engine
	By SerialVelocity
*/
#include "VEBase.hpp"
using namespace VE;

// Resource Cache
ResCache::ResCache( SIZE sizeInMB )
{
	mSize = sizeInMB * 1024 * 1024;
	mAllocated = 0;
}

ResCache::~ResCache()
{
	// Delete each resource
	while( !mLRU.empty() )
		freeOneResource();
}

ResCache *ResCache::getInstance()
{
	// Create an object which is automatically deleted on exit and return
	static ResCache instance( VE_RESCACHE_SIZE );
	return &instance;
}

Resource *ResCache::load( const std::string &filename )
{
	// If the file is already loaded, dont load it again
	CacheList::iterator i = mCache.find( filename );
	if( i != mCache.end() )
	{
		update( i->second.first );
		return i->second.first;
	}

	// Load the data 
	std::ifstream file;
	file.open( filename, std::ios::binary );
	if( !file.is_open() )
	{
		return 0;
	}

	// Get length of file
	file.seekg( 0, std::ios::end );
	SIZE size = (SIZE)file.tellg();
	file.seekg( 0, std::ios::beg );
	
	// Load the data
	char *buffer = allocate( size );
	file.read( buffer, size );
	file.close();

	// Update the pair
	Resource *newResource = new Resource( (void*)buffer, filename, size );
	update( newResource );

	// Add the pair to the map
	mCache[filename] = ResourceDataPair( newResource, buffer );

	// Return the resource pointer
	return newResource;
}

bool ResCache::makeRoom( SIZE size )
{
	// If the size is 0, dont do anything
	if( size == 0 )
		return true;

	// If the size is larger than the cache's size, return fail
	if( size > mSize )
		return false;

	// Free least frequently used resources until there's enough space
	while( ( mSize - mAllocated ) < size )
		freeOneResource();

	// Return true to continue
	return true;
}

SIZE ResCache::size()
{
	// Return the size of the cache
	return mSize;
}

SIZE ResCache::allocated()
{
	// Return the amount of memory allocated
	return mAllocated;
}

ResCache::CacheList &ResCache::getCacheList()
{
	// Return the cache list
	return mCache;
}

ResCache::ResourceList &ResCache::getLRU()
{
	// Return the LRU list
	return mLRU;
}

void ResCache::freeOneResource()
{
	// If the LRU list or the cache list is empty, return
	if( mLRU.empty() || mCache.empty() ) return;

	// Get the least recently used resource on the list
	ResourceList::iterator LRU = mLRU.end();
	LRU--;

	// Get the entry on the cache list and delete data
	CacheList::iterator i = mCache.find( (*LRU)->filename() );
	mAllocated -= (*LRU)->size();
	delete (*i).second.first;
	delete (*i).second.second;

	// Remove entries from the lists
	mLRU.pop_back();
	mCache.erase( i );
}

char *ResCache::allocate( SIZE size )
{
	// Make room
	if( makeRoom( size ) == false )
		return 0;

	// Allocate the memory
	char *buffer = new char[size];
	if( buffer )
		mAllocated += size;
	return buffer;
}

void ResCache::update( Resource *resource )
{
	// Bring the resource to the front of the LRU list
	mLRU.remove( resource );
	mLRU.push_front( resource );
}
