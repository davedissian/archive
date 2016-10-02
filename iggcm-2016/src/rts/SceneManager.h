#pragma once

class Entity;
class PointLight;
class SceneNode;

struct RenderContext;

enum class RenderQueue
{
    ENTITY,
    LIGHT
};

// TODO: Name this to something else like "Renderer"?
class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    // Entities
    Entity* createCube(const string& vs, const string& fs);
    PointLight* createPointLight(float a0, float a1, float a2);

    // Rendering
    void renderEntities(RenderContext* context);
    void renderLights(RenderContext* context);

    // Scene Graph
    SceneNode* createNode();
    void destroyNode(SceneNode* node);
    SceneNode* getRootNode();

    // Render Queue
    void addToRenderQueue(SceneNode* node, RenderQueue queue);

    // Premade meshes
    Mesh* getCubeMesh(bool withTextureCoords);

private:
    SceneNode* mRootNode;
    std::vector<SceneNode*> mNodes;
    std::vector<Entity*> mEntities;

    // Render queues
    std::vector<SceneNode*> mEntityQueue;
    std::vector<SceneNode*> mLightQueue;

    Mesh* mCubeMeshWithTexture;
    Mesh* mCubeMeshWithoutTexture;
    Mesh* mLightSphere;

};