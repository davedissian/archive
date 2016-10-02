#pragma once

struct RenderContext;

class Entity
{
public:
    virtual ~Entity() {}
    virtual void render(RenderContext* renderContext, const glm::mat4& world) = 0;

};