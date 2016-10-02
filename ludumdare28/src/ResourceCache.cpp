// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "ResourceCache.h"

ResourceCache::ResourceCache()
{
}

ResourceCache::~ResourceCache()
{
}

sf::Texture* ResourceCache::getTexture(const string& name)
{
    // Load if it doesn't exist
    auto it = mTextureMap.find(name);
    if (it == mTextureMap.end())
    {
        if (!mTextureMap[name].loadFromFile("media/" + name))
            throw std::runtime_error("Failed to load media/" + name + "!!");
    }

    return &mTextureMap[name];
}
