// source file to create implementation of sokol in static library form

// only build GLES3 as we focus on Emscripten w/ older browser compat
#define SOKOL_GLES3

// implement all sokol libraries
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "imgui.h"
#define SOKOL_IMGUI_IMPL
#include "sokol_imgui.h"
