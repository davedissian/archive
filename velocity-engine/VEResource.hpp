/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

VE_BEGIN

// Resouce
class Resource
{
public:
						Resource( void *data, const std::string &filename, SIZE size );
	virtual				~Resource();

	// Accessors
	void				*data();
	std::string			&filename();
	SIZE				size();

private:
	// Data
	void				*mData;

	// Properties
	std::string			mFilename;
	SIZE				mSize;

};

VE_END
