#pragma once

class Entity;
class SceneManager;

struct RenderContext;

class SceneNode
{
public:
    SceneNode(SceneManager* parentSceneMgr);
    virtual ~SceneNode();

    // Scene graph
    SceneNode* createChildNode();
    void addChild(SceneNode* node);
    void removeNode(SceneNode* node);

    // Sets this scene nodes entity
    void setEntity(Entity* entity);

    // Update
    void update();
    void render(RenderContext* renderContext);

    // Transform
    void setPosition(const glm::vec3& position);
    void setOrientation(const glm::quat& orientation);
    void setScale(const glm::vec3& scale);

    glm::mat4 getWorld();

private:
    SceneManager* mParentSceneMgr;

    // Transform information
    glm::vec3 mPosition;
    glm::quat mOrientation;
    glm::vec3 mScale;

    // Entity object
    Entity* mEntity;

    // Cached matrices
    void updateWorldMatrix();
    glm::mat4 mCachedWorld;
    glm::mat4 mCachedGlobalWorld;

    // Hierarchy
    std::vector<SceneNode*> mChildren;
    SceneNode* mParent;

};