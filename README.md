# Simulator
It's a robot simulator. There's robots! (once we make them.)

## Development

This codebase is developed in modern C++ (as reasonable), using the C++23
standard. The primary target is Web via Emscripten, and tooling isn't currently
provided for building native apps.

## Dependencies

Libraries used by the project are either provided by the system (Core libraries,
OpenGL) or are included in the project tree (Sokol, DearImGui).

CMake is used for building tooling.

The Emscripten SDK is used as the web build toolchain. Install the
[Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) for
your platform.

## Build Instructions
### Prepare the build

`emcmake cmake -B build -S .`

### Build the app

`cmake --build build`

To build in parallel, optionally add the `-j N` flag where N is the number of
threads to build with.

### Run the app

`emrun build/index.html`
