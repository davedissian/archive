/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

class MatrixStack
{
public:
						MatrixStack();
						~MatrixStack();

	// Manipulation
	void				push( Matrix &matrix );
	void				pop();
	Matrix				getTop();

private:
	Matrix				mBase;
	list<Matrix>		mStack;

};

ceEND
