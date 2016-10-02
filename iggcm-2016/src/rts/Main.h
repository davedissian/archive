#pragma once

#include "Context.h"
#include "RenderContext.h"

class RTSGame : public Application
{
public:
    RTSGame();
    virtual ~RTSGame();

    void Startup() override;
    void Shutdown() override;    
    bool Update(float dt) override;
    void Render() override;

private:
    Context mContext;
    RenderContext mRenderContext;
    Framebuffer* mGBuffer;

};