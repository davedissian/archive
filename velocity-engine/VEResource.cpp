/*
	Velocity Engine
	By SerialVelocity
*/
#include "VEBase.hpp"
using namespace VE;

// Resource
Resource::Resource( void *data, const std::string &filename, SIZE size )
{
	mData = data;
	mFilename = filename;
	mSize = size;
}

Resource::~Resource()
{
}

void *Resource::data()
{
	// Return the data
	return mData;
}

std::string &Resource::filename()
{
	// Return the filename
	return mFilename;
}

SIZE Resource::size()
{
	// Return the size in bytes
	return mSize;
}
