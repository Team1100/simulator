#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "imgui.h"

#include "sokol_imgui.h"

static void init(void) {
    sg_desc gfx_desc{};
    gfx_desc.environment = sglue_environment();
    gfx_desc.logger.func = slog_func;
    sg_setup(&gfx_desc);

    simgui_desc_t imgui_desc{};
    imgui_desc.logger.func = slog_func;
    simgui_setup(&imgui_desc);

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
}

static void frame(void) {
    sg_pass pass{};
    pass.action.colors[0] = {
        .load_action = SG_LOADACTION_CLEAR, .clear_value = {0.15f, 0.18f, 0.2f, 1.0f}
    };
    pass.swapchain = sglue_swapchain();

    simgui_frame_desc_t imgui_fdesc{
        .width = sapp_width(),
        .height = sapp_height(),
        .delta_time = sapp_frame_duration(),
        .dpi_scale = sapp_dpi_scale(),
    };
    simgui_new_frame(&imgui_fdesc);

    ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::ShowDemoWindow();

    sg_begin_pass(&pass);
    simgui_render();
    sg_end_pass();
    sg_commit();
}

static void cleanup(void) {
    simgui_shutdown();
    sg_shutdown();
}

static void event(const sapp_event *event) {
    simgui_handle_event(event);
}

sapp_desc sokol_main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    return sapp_desc{
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .sample_count = 4,
        .window_title = "Simulator",
        .icon = {.sokol_default = true},
        .logger = {.func = slog_func},
    };
}
