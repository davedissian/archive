#include "Common.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "Renderable.h"
#include "PointLight.h"

static Mesh* generateBoxWithNormals(float halfSize);
static Mesh* generateBoxWithTextureAndNormals(float halfSize);
static Mesh* generateLightSphere(float radius, int rings, int segments);

SceneManager::SceneManager()
    : mRootNode(nullptr)
{
    mRootNode = createNode();

    mCubeMeshWithTexture = generateBoxWithTextureAndNormals(0.5f);
    mCubeMeshWithoutTexture = generateBoxWithNormals(0.5f);
    mLightSphere = generateLightSphere(1.0f, 8, 8);
}

SceneManager::~SceneManager()
{
    destroyNode(mRootNode);
    for (auto i = mNodes.begin(); i != mNodes.end(); i++)
        destroyNode(*i);
    for (auto i = mEntities.begin(); i != mEntities.end(); i++)
        SAFE_DELETE(*i);
    SAFE_DELETE(mCubeMeshWithoutTexture);
    SAFE_DELETE(mCubeMeshWithTexture);
}

Entity* SceneManager::createCube(const string& vs, const string& fs)
{
    Renderable* renderable = new Renderable(vs, fs, mCubeMeshWithoutTexture);
    mEntities.push_back(renderable);
    return renderable;
}

PointLight* SceneManager::createPointLight(float a0, float a1, float a2)
{
    PointLight* light = new PointLight(a0, a1, a2, mLightSphere);
    mEntities.push_back(light);
    return light;
}

void SceneManager::renderEntities(RenderContext* context)
{
    // TODO: Make this more cache friendly
    for (auto i = mEntityQueue.begin(); i != mEntityQueue.end(); i++)
    {
        (*i)->render(context);
    }
}

void SceneManager::renderLights(RenderContext* context)
{
    // TODO: Make this more cache friendly
    for (auto i = mLightQueue.begin(); i != mLightQueue.end(); i++)
    {
        (*i)->render(context);
    }
}

SceneNode* SceneManager::createNode()
{
    SceneNode* node = new SceneNode(this);
    mNodes.push_back(node);
    return node;
}

void SceneManager::destroyNode(SceneNode* node)
{
    SAFE_DELETE(node);
}

SceneNode* SceneManager::getRootNode()
{
    return mRootNode;
}

void SceneManager::addToRenderQueue(SceneNode* node, RenderQueue queue)
{
    if (queue == RenderQueue::ENTITY)
    {
        mEntityQueue.push_back(node);
    }
    else if (queue == RenderQueue::LIGHT)
    {
        mLightQueue.push_back(node);
    }
}

Mesh* SceneManager::getCubeMesh(bool withTextureCoords)
{
    return withTextureCoords ? mCubeMeshWithTexture : mCubeMeshWithoutTexture;
}

static Mesh* generateBoxWithNormals(float halfSize)
{
    return new Mesh({
        // Position                     | Normals
        -halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f,
        halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f,
        halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f,
        halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f,
        -halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f,
        -halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f,

        -halfSize, -halfSize,  halfSize,  0.0f,  0.0f, 1.0f,
        halfSize, -halfSize,  halfSize,  0.0f,  0.0f, 1.0f,
        halfSize,  halfSize,  halfSize,  0.0f,  0.0f, 1.0f,
        halfSize,  halfSize,  halfSize,  0.0f,  0.0f, 1.0f,
        -halfSize,  halfSize,  halfSize,  0.0f,  0.0f, 1.0f,
        -halfSize, -halfSize,  halfSize,  0.0f,  0.0f, 1.0f,

        -halfSize,  halfSize,  halfSize, -1.0f,  0.0f,  0.0f,
        -halfSize,  halfSize, -halfSize, -1.0f,  0.0f,  0.0f,
        -halfSize, -halfSize, -halfSize, -1.0f,  0.0f,  0.0f,
        -halfSize, -halfSize, -halfSize, -1.0f,  0.0f,  0.0f,
        -halfSize, -halfSize,  halfSize, -1.0f,  0.0f,  0.0f,
        -halfSize,  halfSize,  halfSize, -1.0f,  0.0f,  0.0f,

        halfSize,  halfSize,  halfSize,  1.0f,  0.0f,  0.0f,
        halfSize,  halfSize, -halfSize,  1.0f,  0.0f,  0.0f,
        halfSize, -halfSize, -halfSize,  1.0f,  0.0f,  0.0f,
        halfSize, -halfSize, -halfSize,  1.0f,  0.0f,  0.0f,
        halfSize, -halfSize,  halfSize,  1.0f,  0.0f,  0.0f,
        halfSize,  halfSize,  halfSize,  1.0f,  0.0f,  0.0f,

        -halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f,
        halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f,
        halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f,
        halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f,
        -halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f,
        -halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f,

        -halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f,
        halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f,
        halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f,
        halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f,
        -halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f,
        -halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f
    }, {{3, GL_FLOAT}, {3, GL_FLOAT}});
}

static Mesh* generateBoxWithTextureAndNormals(float halfSize)
{
    return new Mesh({
        // Position                     | Normals            | UVs
        -halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -halfSize, -halfSize,  halfSize,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
        halfSize, -halfSize,  halfSize,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
        halfSize,  halfSize,  halfSize,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        halfSize,  halfSize,  halfSize,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        -halfSize,  halfSize,  halfSize,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
        -halfSize, -halfSize,  halfSize,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

        -halfSize,  halfSize,  halfSize, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -halfSize,  halfSize, -halfSize, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -halfSize, -halfSize, -halfSize, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -halfSize, -halfSize, -halfSize, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -halfSize, -halfSize,  halfSize, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -halfSize,  halfSize,  halfSize, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        halfSize,  halfSize,  halfSize,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        halfSize,  halfSize, -halfSize,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        halfSize, -halfSize, -halfSize,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        halfSize, -halfSize, -halfSize,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        halfSize, -halfSize,  halfSize,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        halfSize,  halfSize,  halfSize,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        -halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        -halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        -halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

        -halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        -halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
    }, {{3, GL_FLOAT}, {3, GL_FLOAT}, {2, GL_FLOAT}});
}

static Mesh* generateLightSphere(float radius, int rings, int segments)
{
    vector<GLfloat> vertexData;
    vector<GLuint> indexData;

    // Generate the group of rings for the sphere
    float deltaRingAngle = (float)M_PI / rings;
    float deltaSegAngle = 2.0f * (float)M_PI / segments;
    unsigned short vertexIndex = 0;
    for (int ring = 0; ring <= rings; ring++)
    {
        float r0 = radius * sinf(ring * deltaRingAngle);
        float y0 = radius * cosf(ring * deltaRingAngle);

        // Generate the group of segments for the current ring
        for (int seg = 0; seg <= segments; seg++)
        {
            float x0 = r0 * sinf(seg * deltaSegAngle);
            float z0 = r0 * cosf(seg * deltaSegAngle);

            // Add one vertex to the strip which makes up the sphere
            vertexData.push_back(x0);
            vertexData.push_back(y0);
            vertexData.push_back(z0);

            if (ring != rings)
            {
                // each vertex (except the last) has six indices pointing to it
                indexData.push_back(vertexIndex + segments + 1);
                indexData.push_back(vertexIndex);
                indexData.push_back(vertexIndex + segments);
                indexData.push_back(vertexIndex + segments + 1);
                indexData.push_back(vertexIndex + 1);
                indexData.push_back(vertexIndex);
                vertexIndex++;
            }
        }
    }

    return new Mesh(vertexData, indexData, {{3, GL_FLOAT}});
}