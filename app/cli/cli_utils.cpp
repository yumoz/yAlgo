#include "cli_utils.h"
#include <sstream>
#include <stdexcept>
#include "../../sdk/math/math_utils.h"

using namespace yalgo::earth;
using namespace yalgo::math;

std::vector<double> parseDoubleList(const std::string& str) {
    std::vector<double> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        result.push_back(std::stod(token));
    }
    return result;
}

EarthPoint parsePoint2D(const std::string& str) {
    auto vals = parseDoubleList(str);
    if (vals.size() < 2) {
        throw std::invalid_argument("需要至少两个数值 (经度,纬度)");
    }
    return EarthPoint(vals[0], vals[1]);
}

EarthPoint parsePoint3D(const std::string& str) {
    auto vals = parseDoubleList(str);
    if (vals.size() < 3) {
        throw std::invalid_argument("需要三个数值 (经度,纬度,高度)");
    }
    return EarthPoint(vals[0], vals[1], vals[2]);
}

double toWatts(double value, const std::string& unit) {
    if (unit == "w")   return value;
    if (unit == "kw")  return kilowattsToWatts(value);
    if (unit == "hp")  return horsepowerToWatts(value);
    if (unit == "dbw") return dbWToWatts(value);
    if (unit == "dbm") return dbmToWatts(value);
    throw std::invalid_argument("不支持的功率单位: " + unit);
}

double fromWatts(double value, const std::string& unit) {
    if (unit == "w")   return value;
    if (unit == "kw")  return wattsToKilowatts(value);
    if (unit == "hp")  return wattsToHorsepower(value);
    if (unit == "dbw") return wattsToDbW(value);
    if (unit == "dbm") return wattsToDbm(value);
    throw std::invalid_argument("不支持的功率单位: " + unit);
}
