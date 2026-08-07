#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "imgui.h"

#include "sokol_imgui.h"

#include "sol/sol.hpp"

// Holder of graphics state objects
struct gfx_state_t {
    sg_pass pass;
} gfx;

// Holder of Lua + console state
struct lua_state_t {
    std::array<char, 512> console_buf;
    std::vector<std::string> console_out;
    sol::state state;
} lua;

static void lua_print(sol::variadic_args va);
static void reset_lua_state();
static void show_console();
static void init(void);
static void frame(void);
static void event(const sapp_event *event);
static void cleanup(void);

// Custom Lua print function to print to the UI console rather than stdout
static void lua_print(sol::variadic_args va) {
    std::string str;
    for (auto v : va) {
        // use Lua's tostring to get a string, then convert to std::string
        auto vstr = lua.state["tostring"](v.as<sol::object>()).get<std::string>();
        str += vstr + "\t";
    }
    lua.console_out.push_back(str);
}

// (Re)start Lua state and prepare environment
static void reset_lua_state() {
    // Reset state
    lua.state = sol::state();
    // Open only core libraries (no I/O, OS, etc.)
    lua.state.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
    // Override print with custom one for console output
    lua.state["print"] = lua_print;
}

// Display the Lua console with multi-line input, output, and controls
static void show_console() {
    if (ImGui::Begin("Lua Test Console")) {
        // World's worst code editor
        ImGui::InputTextMultiline("Script Input", lua.console_buf.data(), lua.console_buf.size(),
                                  ImVec2(0, 0), ImGuiInputTextFlags_AllowTabInput);

        // Character counter
        std::string_view str(lua.console_buf.data());
        ImGui::Text("%zu/%zu", str.size(), lua.console_buf.size());

        // Safely execute script and output errors to the console
        if (ImGui::Button("Execute")) {
            auto res = lua.state.safe_script(str, sol::script_pass_on_error);
            if (!res.valid()) {
                sol::error err = res;
                lua.console_out.push_back(std::string("[error] ") + err.what());
            }
        }

        ImGui::SameLine();

        // Globals persist between script executions, resetting clears them
        if (ImGui::Button("Reset Lua"))
            reset_lua_state();

        ImGui::SameLine();

        if (ImGui::Button("Clear Output"))
            lua.console_out.clear();

        ImGui::Separator();

        // Formatted output, each message is a separate widget
        if (ImGui::BeginChild("Output", ImVec2(0, ImGui::GetContentRegionAvail().y),
                              ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar)) {
            for (auto i : lua.console_out) {
                if (i.starts_with("[error]"))
                    ImGui::TextColored({1.0f, 0.4f, 0.2f, 1.0f}, "%s", i.c_str());
                else
                    ImGui::Text("%s", i.c_str());
            }

            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

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

    gfx.pass = sg_pass{};
    gfx.pass.action.colors[0] = {
        .load_action = SG_LOADACTION_CLEAR,
        .clear_value = {0.15f, 0.18f, 0.2f, 1.0f},
    };
    reset_lua_state();
}

static void frame(void) {
    gfx.pass.swapchain = sglue_swapchain();

    simgui_frame_desc_t imgui_fdesc{
        .width = sapp_width(),
        .height = sapp_height(),
        .delta_time = sapp_frame_duration(),
        .dpi_scale = sapp_dpi_scale(),
    };
    simgui_new_frame(&imgui_fdesc);

    ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::ShowDemoWindow();

    show_console();

    sg_begin_pass(&gfx.pass);
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
