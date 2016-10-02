#include "Common.h"
#include "SceneNode.h"
#include "SceneManager.h"
#include "Entity.h"

SceneNode::SceneNode(SceneManager* parentSceneMgr)
    : mParentSceneMgr(parentSceneMgr),
      mPosition(),
      mOrientation(),
      mScale(1.0f, 1.0f, 1.0f),
      mEntity(nullptr),
      mCachedWorld(1.0f),
      mCachedGlobalWorld(1.0f),
      mParent(nullptr)
{
}

SceneNode::~SceneNode()
{
}

SceneNode* SceneNode::createChildNode()
{
    SceneNode* result = mParentSceneMgr->createNode();
    addChild(result);
    return result;
}

void SceneNode::addChild(SceneNode* node)
{
    mChildren.push_back(node);
    node->mParent = this;
}

void SceneNode::removeNode(SceneNode* node)
{
    // TODO: implement this
}

void SceneNode::setEntity(Entity* entity)
{
    mEntity = entity;
}

void SceneNode::update()
{
    // Update global world matrix
    updateWorldMatrix();
    if (mParent)
    {
        mCachedGlobalWorld = mParent->mCachedGlobalWorld * mCachedWorld;
    }
    else
    {
        mCachedGlobalWorld = mCachedWorld;
    }

    // Update children
    for (auto i = mChildren.begin(); i != mChildren.end(); i++)
    {
        (*i)->update();
    }
}

void SceneNode::render(RenderContext* renderContext)
{
    if (mEntity)
    {
        mEntity->render(renderContext, mCachedGlobalWorld);
    }
}

void SceneNode::setPosition(const glm::vec3& position)
{
    mPosition = position;
}

void SceneNode::setOrientation(const glm::quat& orientation)
{
    mOrientation = orientation;
}

void SceneNode::setScale(const glm::vec3& scale)
{
    mScale = scale;
}

glm::mat4 SceneNode::getWorld()
{
    updateWorldMatrix();
    return mCachedWorld;
}

void SceneNode::updateWorldMatrix()
{
    mCachedWorld = glm::translate(mPosition) * glm::mat4_cast(mOrientation) * glm::scale(mScale);
}