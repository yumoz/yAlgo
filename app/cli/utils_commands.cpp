#include "utils_commands.h"
#include "../../3rd/cli/json.hpp"
#include "../../sdk/utils/exe_path.h"

using namespace yalgo::utils;
using json = nlohmann::json;

static CLI::App* utilsCmd = nullptr;
static CLI::App* execPathCmd = nullptr;
static CLI::App* sysTypeCmd = nullptr;

CLI::App* registerUtilsCommands(CLI::App& app) {
    utilsCmd = app.add_subcommand("utils", "工具");
    utilsCmd->require_subcommand();

    execPathCmd = utilsCmd->add_subcommand("exec-path", "获取可执行文件路径");

    sysTypeCmd = utilsCmd->add_subcommand("system-type", "获取系统类型");

    return utilsCmd;
}

int executeUtilsCommand() {
    json output;

    if (execPathCmd->parsed()) {
        output["executable_path"] = ExePath::getExecutablePath();
        output["executable_dir"]  = ExePath::getExecutableDir();
    }

    if (sysTypeCmd->parsed()) {
        output["system_type"] = ExePath::getSystemType();
    }

    if (!output.is_null()) {
        std::cout << output.dump(4) << std::endl;
    }
    return 0;
}
