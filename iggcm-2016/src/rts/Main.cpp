#include "Common.h"
#include "Main.h"

#include "Context.h"
#include "RenderContext.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "Renderable.h"
#include "PointLight.h"

DEFINE_MAIN_FUNCTION(RTSGame);

RTSGame::RTSGame()
{
}

RTSGame::~RTSGame()
{
}

SceneNode* node;

void RTSGame::Startup()
{
    mContext.sceneMgr = new SceneManager();
    mRenderContext.view = glm::mat4();
    mRenderContext.projection = glm::perspective(45.0f, (float)WIDTH / HEIGHT, 0.1f, 10000.0f);

    // Set up the g-buffer
    mGBuffer = new Framebuffer(mWindowWidth, mWindowHeight, 3);

    // Create a nice little unit
    Entity* cube = mContext.sceneMgr->createCube("media/default.vs", "media/default.fs");
    node = mContext.sceneMgr->getRootNode()->createChildNode();
    mContext.sceneMgr->addToRenderQueue(node, RenderQueue::ENTITY);
    node->setEntity(cube);
    node->setPosition(glm::vec3(0.0f, 0.0f, -3.0f));

    SceneNode* left = node->createChildNode();
    mContext.sceneMgr->addToRenderQueue(left, RenderQueue::ENTITY);
    left->setEntity(cube);
    left->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
    left->setScale(glm::vec3(0.3f));

    SceneNode* right = node->createChildNode();
    mContext.sceneMgr->addToRenderQueue(right, RenderQueue::ENTITY);
    right->setEntity(cube);
    right->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    right->setScale(glm::vec3(0.3f));

    // Create a light
    SceneNode* lightNode = mContext.sceneMgr->getRootNode()->createChildNode();
    PointLight* light = mContext.sceneMgr->createPointLight(0, 0, 0);
    mContext.sceneMgr->addToRenderQueue(lightNode, RenderQueue::LIGHT);
    lightNode->setEntity(light);
    node->setPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
}

void RTSGame::Shutdown()
{
}

bool RTSGame::Update(float dt)
{
    static float rotation = 0.0f;
    rotation += dt;
    node->setOrientation(glm::angleAxis(rotation, glm::vec3(0.0f, 1.0f, 0.0f)));
    return true;
}

void RTSGame::Render()
{
    // Update scene graph
    mContext.sceneMgr->getRootNode()->update();
    mRenderContext.viewProjection = mRenderContext.projection * mRenderContext.view;

    // Render
    mGBuffer->Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mContext.sceneMgr->renderEntities(&mRenderContext);
    glDisable(GL_DEPTH_TEST);

    // Draw lights
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    glClear(GL_COLOR_BUFFER_BIT);
    {
        // Bind G-Buffer
        mGBuffer->GetColourBuffer(0)->Bind(0);
        mGBuffer->GetColourBuffer(1)->Bind(1);
        mGBuffer->GetColourBuffer(2)->Bind(2);

        // draw lights
        mContext.sceneMgr->renderLights(&mRenderContext);
    }
    glDisable(GL_BLEND);


    // Check for GL errors
    GLuint err = glGetError();
    if (err != 0)
    {
        ERROR << "glGetError() returned " << err << endl;
    }
}
