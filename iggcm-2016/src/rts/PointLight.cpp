#include "Common.h"
#include "PointLight.h"
#include "RenderContext.h"

PointLight::PointLight(float a0, float a1, float a2, Mesh* mesh)
    : mShader(new Shader("media/light.vs", "media/point-light.fs")),
      mMesh(mesh)
{
    mAtten[0] = a0;
    mAtten[1] = a1;
    mAtten[2] = a2;

    mShader->Bind();
    mShader->SetUniform("screenSize", glm::vec2(WIDTH, HEIGHT));
    mShader->SetUniform("gb0", 0);
    mShader->SetUniform("gb1", 1);
    mShader->SetUniform("gb2", 2);
    mShader->SetUniform("constant", a0);
    mShader->SetUniform("linear", a1);
    mShader->SetUniform("exponent", a2);

    // Calculate range
    // Solve 'a2 * d^2 + a1 * d + a0 = 256' for d
    // 256 is the number of distinct light levels in an 8 bit component (2^8)
    float range;
    if (a2 == 0.0f)
    {
        if (a1 == 0.0f)
        {
            // For constant attenuation point lights, set the range to infinity
            range = 100000000000000000.0f;
        }
        else
        {
            range = (256.0f - a0) / a1;
        }
    }
    else
    {
        range = (-a1 + sqrtf(a1 * a1 - 4.0f * a2 * (a0 - 256.0f))) / (2.0f * a2);
    }

    // Build scale matrix
    mRangeScale = glm::scale(glm::vec3(range));
}

PointLight::~PointLight()
{
    SAFE_DELETE(mShader);
}

void PointLight::render(RenderContext* context, const glm::mat4& world)
{
    mShader->Bind();
    mShader->SetUniform("wvp", context->viewProjection * world);
    mShader->SetUniform("lightPosition", glm::vec3(world[3])); // 4th column is the position
    mMesh->Bind();
    mMesh->Draw();
}
