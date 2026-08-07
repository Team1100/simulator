# Simulator
It's a robot simulator. There's robots! (once we make them.)

## Development

This codebase is developed in modern C++ (as reasonable), using the C++23
standard. The primary target is Web via Emscripten, and tooling isn't currently
provided for building native apps.

## Dependencies

Libraries used:
- [sokol](https://github.com/floooh/sokol) for windowing, graphics, and more.
- [DearImGui](https://github.com/ocornut/imgui) for debug immediate-mode UI
- [Lua](https://www.lua.org/) for embedded scripting
- [sol2](https://github.com/ThePhD/sol2) for a better way to use Lua

Toolchain used:
- [CMake](https://www.cmake.org/) for build system
- [Emscripten](https://emscripten.org/) for targeting Web

## Build Instructions
### Prepare the build

`emcmake cmake -B build -S .`

### Build the app

`cmake --build build`

To build in parallel, optionally add the `-j N` flag where N is the number of
threads to build with.

### Run the app

`emrun build/index.html`
