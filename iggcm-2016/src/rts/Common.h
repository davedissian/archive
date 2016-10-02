#pragma once

// Framework
#include "framework/Common.h"
#include "framework/Application.h"
#include "framework/Framebuffer.h"
#include "framework/Mesh.h"
#include "framework/Shader.h"
#include "framework/Texture.h"

// Macros
#define WIDTH 1024
#define HEIGHT 768
#define SAFE_DELETE(x) if (x) { delete (x); x = nullptr; }

// Utility functions
// TODO
