/*
	Celestial Engine
	Created by SerialVelocity
*/
#include "celestialBase.h"

ceBEGIN

MatrixStack::MatrixStack()
{
	mBase = MatIdentity();
}

MatrixStack::~MatrixStack()
{
	mStack.clear();
}

void MatrixStack::push( Matrix &matrix )
{
	mStack.push_back( matrix );
}

void MatrixStack::pop()
{
	mStack.pop_back();
}

Matrix MatrixStack::getTop()
{
	Matrix result = mBase;

	// Multiply by each matrix in the stack from bottom up
	if( !mStack.empty() )
	{
		for( list<Matrix>::iterator i = mStack.begin(); i != mStack.end(); i++ )
			result *= (*i);
	}

	return result;
}

ceEND
