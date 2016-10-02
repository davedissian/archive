#pragma once

#include "Entity.h"

class PointLight : public Entity
{
public:
    PointLight(float a0, float a1, float a2, Mesh* mesh);
    virtual ~PointLight();

    void render(RenderContext* renderContext, const glm::mat4& world) override;

private:
    Shader* mShader;
    Mesh* mMesh;

    float mAtten[3];
    glm::mat4 mRangeScale;

};
