# yAlgo — agent instructions

## Project

C++17 CMake project, 4 **shared libraries** (`yalgo_log`, `yalgo_math`, `yalgo_utils`, `yalgo_earth`) under `sdk/`, a CLI tool `yalk` under `examples/cli/`, and a Python MCP server at `tools/mcp_server.py`.

Namespaces: `yalgo::log`, `yalgo::math`, `yalgo::utils`, `yalgo::earth`.

## Build (CMake presets are the single source of truth)

```bash
cmake --preset windows-release    # MSVC VS 17 2022
cmake --build --preset windows-release

cmake --preset windows-debug
cmake --build --preset windows-debug

# Presets also exist for linux-release/debug, macos-release/debug, kylin-arm-release/debug
```

Manual: `cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja && cmake --build build`

Install: `cmake --install build` → `install/` prefix.

## Test

No framework — tests return 0 (pass) / 1 (fail) from `main()`. Run via CTest:

```bash
ctest --build-config Release --test-dir build        # all
ctest -V --build-config Release --test-dir build      # verbose
ctest -R unit --build-config Release --test-dir build # unit only
ctest -R integration --build-config Release --test-dir build  # integration only
```

Registered tests: `unit_log`, `unit_utils`, `unit_math`, `unit_earth`, `integration_modules`.

`examples/` are manual demos, NOT registered as CTest tests.

## Key conventions

- **Doxygen** comments: `@brief`, `@param`, `@return`, `@file`, `@author`, `@date` style.
- **UTF-8** everywhere; MSVC gets `/utf-8` flag automatically.
- **Output layout**: `bin/<os>/release|debug/` for executables/DLLs, `lib/<os>/release|debug/` for import libs. Debug builds append `d` suffix (e.g. `yalgo_logd.dll`).
- **Thread safety**: `yalgo::log::AsyncLogger` is thread-safe; value types like `EarthPoint` are NOT.
- **Mercator** projection limited to latitude [-85.05, 85.05].
- **MacOS**: not fully tested.
- **No lint / format / static analysis** tools configured.
- **No package manager**; 3rd-party headers (`CLI11.hpp`, `nlohmann/json.hpp`) live in `3rd/cli/`, which is `.gitignore`d — must be supplied externally.

## Gotcha

`YuMakeSDK.cmake` / `YuMakeApp.cmake` set their own output paths, but the root `CMakeLists.txt:56-59` overrides `CMAKE_RUNTIME/LIBRARY/ARCHIVE_OUTPUT_DIRECTORY_<CONFIG>`. The root CMakeLists.txt wins — the module paths are effectively ignored.

## Related docs

- `.trae/skills/yAlgo/SKILL.md` — full module API reference, Doxygen style guide, platform notes.
- `doc/cli_mcp_guide.md` — `yalk` CLI usage and MCP server setup (requires `pip install mcp`).
