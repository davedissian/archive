#include "Common.h"
#include "Renderable.h"
#include "RenderContext.h"

Renderable::Renderable(const string& vs, const string& fs, Mesh* mesh)
    : mShader(new Shader(vs, fs)),
      mMesh(mesh)
{
}

Renderable::~Renderable()
{
    SAFE_DELETE(mShader);
}

void Renderable::render(RenderContext* context, const glm::mat4& world)
{
    mShader->Bind();
    mShader->SetUniform("wvp", context->viewProjection * world);
    mShader->SetUniform("world", world);
    mMesh->Bind();
    mMesh->Draw();
}
