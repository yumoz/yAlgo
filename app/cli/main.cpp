/**
 * @file main.cpp
 * @brief yAlgo SDK 命令行工具入口
 * @author yAlgo Team
 * @date 2026-05-17
 */

#include "../../3rd/cli/CLI11.hpp"
#include "../../3rd/cli/json.hpp"
#include "earth_commands.h"
#include "math_commands.h"
#include "utils_commands.h"

#include <iostream>

int main(int argc, char** argv) {
    CLI::App app{"yAlgo SDK 命令行工具"};
    app.name("yalk");

    auto* earthCmd  = registerEarthCommands(app);
    auto* mathCmd   = registerMathCommands(app);
    auto* utilsCmd  = registerUtilsCommands(app);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    try {
        if (earthCmd->parsed())  return executeEarthCommand();
        if (mathCmd->parsed())   return executeMathCommand();
        if (utilsCmd->parsed())  return executeUtilsCommand();
    } catch (const std::exception& e) {
        nlohmann::json err;
        err["error"] = e.what();
        std::cerr << err.dump(4) << std::endl;
        return 1;
    }

    return 0;
}
