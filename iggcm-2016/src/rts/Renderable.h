#pragma once

#include "Entity.h"

class Renderable : public Entity
{
public:
    Renderable(const string& vs, const string& fs, Mesh* mesh);
    virtual ~Renderable();

    void render(RenderContext* context, const glm::mat4& world) override;

private:
    Shader* mShader;
    Mesh* mMesh;

};
