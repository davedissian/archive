/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

class Mesh
{
public:
	// Loading
	virtual void		load( const std::string &asset, Shader *shader ) = 0;

};

ceEND
