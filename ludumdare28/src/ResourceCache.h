// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef ResourceCache_h__
#define ResourceCache_h__

#include "Singleton.h"

class ResourceCache : public Singleton<ResourceCache>
{
public:
    ResourceCache();
    ~ResourceCache();

    sf::Texture* getTexture(const string& name);    // load from media/

private:
    map<string, sf::Texture> mTextureMap;
};

#endif    // ResourceCache_h__
