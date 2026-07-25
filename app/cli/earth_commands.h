#pragma once

#include "../../3rd/cli/CLI11.hpp"

CLI::App* registerEarthCommands(CLI::App& app);
int executeEarthCommand();
