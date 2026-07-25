#pragma once

#include "../../3rd/cli/CLI11.hpp"

CLI::App* registerMathCommands(CLI::App& app);
int executeMathCommand();
