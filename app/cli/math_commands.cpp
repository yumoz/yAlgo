#include "math_commands.h"
#include "cli_utils.h"
#include "../../3rd/cli/json.hpp"
#include "../../sdk/math/math_utils.h"

using namespace yalgo::math;
using json = nlohmann::json;

static CLI::App* mathCmd = nullptr;
static CLI::App* angleCmd = nullptr;
static CLI::App* powerCmd = nullptr;

static double angleValue;
static std::string angleFrom, angleTo;
static double powerValue;
static std::string powerFrom, powerTo;

CLI::App* registerMathCommands(CLI::App& app) {
    mathCmd = app.add_subcommand("math", "数学工具");
    mathCmd->require_subcommand();

    angleCmd = mathCmd->add_subcommand("angle", "角度转换");
    angleCmd->add_option("--value", angleValue, "角度值")->required();
    angleCmd->add_option("--from", angleFrom, "deg|rad")->required();
    angleCmd->add_option("--to", angleTo, "deg|rad")->required();

    powerCmd = mathCmd->add_subcommand("power", "功率单位转换");
    powerCmd->add_option("--value", powerValue, "功率值")->required();
    powerCmd->add_option("--from", powerFrom, "w|kw|hp|dbw|dbm")->required();
    powerCmd->add_option("--to", powerTo, "w|kw|hp|dbw|dbm")->required();

    return mathCmd;
}

int executeMathCommand() {
    json output;

    if (angleCmd->parsed()) {
        double result = 0.0;
        if (angleFrom == "deg" && angleTo == "rad") {
            result = degreesToRadians(angleValue);
        } else if (angleFrom == "rad" && angleTo == "deg") {
            result = radiansToDegrees(angleValue);
        } else if (angleFrom == angleTo) {
            result = angleValue;
        } else {
            throw std::invalid_argument("不支持的角度单位: " + angleFrom + " -> " + angleTo);
        }
        output["result"] = result;
        output["unit"] = angleTo;
    }

    if (powerCmd->parsed()) {
        double inWatts = toWatts(powerValue, powerFrom);
        double result  = fromWatts(inWatts, powerTo);
        output["result"] = result;
        output["unit"] = powerTo;
    }

    if (!output.is_null()) {
        std::cout << output.dump(4) << std::endl;
    }
    return 0;
}
