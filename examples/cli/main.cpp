/**
 * @file main.cpp
 * @brief yAlgo SDK 命令行工具入口
 * @author yAlgo Team
 * @date 2026-05-17
 */

#include "../../3rd/cli/CLI11.hpp"
#include "../../3rd/cli/json.hpp"
#include "../../sdk/earth/earth_point.h"
#include "../../sdk/earth/earth_converter.h"
#include "../../sdk/earth/earth_geometry.h"
#include "../../sdk/math/math_utils.h"
#include "../../sdk/utils/exec_path_utils.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace yalgo::earth;
using namespace yalgo::math;
using namespace yalgo::utils;

using json = nlohmann::json;

static std::vector<double> parseDoubleList(const std::string& str) {
    std::vector<double> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        result.push_back(std::stod(token));
    }
    return result;
}

static EarthPoint parsePoint2D(const std::string& str) {
    auto vals = parseDoubleList(str);
    if (vals.size() < 2) {
        throw std::invalid_argument("需要至少两个数值 (经度,纬度)");
    }
    return EarthPoint(vals[0], vals[1]);
}

static EarthPoint parsePoint3D(const std::string& str) {
    auto vals = parseDoubleList(str);
    if (vals.size() < 3) {
        throw std::invalid_argument("需要三个数值 (经度,纬度,高度)");
    }
    return EarthPoint(vals[0], vals[1], vals[2]);
}

static double toWatts(double value, const std::string& unit) {
    if (unit == "w")   return value;
    if (unit == "kw")  return kilowattsToWatts(value);
    if (unit == "hp")  return horsepowerToWatts(value);
    if (unit == "dbw") return dbWToWatts(value);
    if (unit == "dbm") return dbmToWatts(value);
    throw std::invalid_argument("不支持的功率单位: " + unit);
}

static double fromWatts(double value, const std::string& unit) {
    if (unit == "w")   return value;
    if (unit == "kw")  return wattsToKilowatts(value);
    if (unit == "hp")  return wattsToHorsepower(value);
    if (unit == "dbw") return wattsToDbW(value);
    if (unit == "dbm") return wattsToDbm(value);
    throw std::invalid_argument("不支持的功率单位: " + unit);
}

int main(int argc, char** argv) {
    CLI::App app{"yAlgo SDK \u547d\u4ee4\u884c\u5de5\u5177"};
    app.name("yalk");

    auto* earthCmd = app.add_subcommand("earth", "\u5730\u7403\u5750\u6807\u76f8\u5173\u64cd\u4f5c");
    auto* mathCmd  = app.add_subcommand("math", "\u6570\u5b66\u5de5\u5177");
    auto* utilsCmd = app.add_subcommand("utils", "\u5de5\u5177");

    earthCmd->require_subcommand();
    mathCmd->require_subcommand();
    utilsCmd->require_subcommand();

    auto* distanceCmd = earthCmd->add_subcommand("distance", "\u8ba1\u7b97\u4e24\u70b9\u8ddd\u79bb");
    std::string distFromStr, distToStr, distType = "haversine";
    distanceCmd->add_option("--from", distFromStr, "\u7ecf\u7eac\u5ea6, \u5982 116.4,39.9")->required();
    distanceCmd->add_option("--to", distToStr, "\u7ecf\u7eac\u5ea6, \u5982 121.5,31.2")->required();
    distanceCmd->add_option("--type", distType, "haversine|vincenty|straight, \u9ed8\u8ba4 haversine");

    auto* convertCmd = earthCmd->add_subcommand("convert", "\u5750\u6807\u7cfb\u8f6c\u6362");
    std::string convertInputStr, convertFrom = "wgs84", convertTo;
    convertCmd->add_option("--input", convertInputStr, "\u7ecf\u5ea6,\u7eac\u5ea6,\u9ad8\u5ea6")->required();
    convertCmd->add_option("--from", convertFrom, "wgs84, \u9ed8\u8ba4");
    convertCmd->add_option("--to", convertTo, "ecef|utm|mercator")->required();

    auto* bearingCmd = earthCmd->add_subcommand("bearing", "\u8ba1\u7b97\u65b9\u4f4d\u89d2");
    std::string bearFromStr, bearToStr;
    bearingCmd->add_option("--from", bearFromStr, "\u7ecf\u7eac\u5ea6, \u5982 116.4,39.9")->required();
    bearingCmd->add_option("--to", bearToStr, "\u7ecf\u7eac\u5ea6, \u5982 121.5,31.2")->required();

    auto* visibleCmd = earthCmd->add_subcommand("is-visible", "\u901a\u89c6\u5224\u65ad");
    std::string visFromStr, visToStr;
    visibleCmd->add_option("--from", visFromStr, "\u7ecf\u5ea6,\u7eac\u5ea6,\u9ad8\u5ea6")->required();
    visibleCmd->add_option("--to", visToStr, "\u7ecf\u5ea6,\u7eac\u5ea6,\u9ad8\u5ea6")->required();

    auto* inPolyCmd = earthCmd->add_subcommand("is-point-in-polygon", "\u70b9\u5728\u591a\u8fb9\u5f62\u5185\u5224\u65ad");
    std::string polyPointStr, polyPolygonStr, polyProjType = "utm";
    inPolyCmd->add_option("--point", polyPointStr, "\u7ecf\u7eac\u5ea6, \u5982 116.4,39.9")->required();
    inPolyCmd->add_option("--polygon", polyPolygonStr, "JSON\u6570\u7ec4 [[lng,lat],...]")->required();
    inPolyCmd->add_option("--projection", polyProjType, "utm|mercator");

    auto* angleCmd = mathCmd->add_subcommand("angle", "\u89d2\u5ea6\u8f6c\u6362");
    double angleValue;
    std::string angleFrom, angleTo;
    angleCmd->add_option("--value", angleValue, "\u89d2\u5ea6\u503c")->required();
    angleCmd->add_option("--from", angleFrom, "deg|rad")->required();
    angleCmd->add_option("--to", angleTo, "deg|rad")->required();

    auto* powerCmd = mathCmd->add_subcommand("power", "\u529f\u7387\u5355\u4f4d\u8f6c\u6362");
    double powerValue;
    std::string powerFrom, powerTo;
    powerCmd->add_option("--value", powerValue, "\u529f\u7387\u503c")->required();
    powerCmd->add_option("--from", powerFrom, "w|kw|hp|dbw|dbm")->required();
    powerCmd->add_option("--to", powerTo, "w|kw|hp|dbw|dbm")->required();

    auto* execPathCmd = utilsCmd->add_subcommand("exec-path", "\u83b7\u53d6\u53ef\u6267\u884c\u6587\u4ef6\u8def\u5f84");

    auto* sysTypeCmd = utilsCmd->add_subcommand("system-type", "\u83b7\u53d6\u7cfb\u7edf\u7c7b\u578b");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    json output;
    int exitCode = 0;

    try {
        if (distanceCmd->parsed()) {
            auto from = parsePoint2D(distFromStr);
            auto to   = parsePoint2D(distToStr);
            double result = 0.0;
            if (distType == "haversine") {
                result = distance(from, to);
            } else if (distType == "vincenty") {
                result = vincentyDistance(from, to);
            } else if (distType == "straight") {
                result = straightLineDistance(from, to);
            } else {
                throw std::invalid_argument("\u4e0d\u652f\u6301\u7684\u8ddd\u79bb\u7c7b\u578b: " + distType);
            }
            output["distance"] = result;
            output["unit"]     = "m";
            output["method"]   = distType;
        }

        if (convertCmd->parsed()) {
            if (convertFrom != "wgs84") {
                throw std::invalid_argument("\u4ec5\u652f\u6301\u4ece wgs84 \u8f6c\u6362");
            }
            auto pt = parsePoint3D(convertInputStr);
            EarthConverter converter;
            output["from"] = convertFrom;
            if (convertTo == "ecef") {
                auto ecef = converter.wgs84ToECEF(pt);
                output["x"] = ecef.x;
                output["y"] = ecef.y;
                output["z"] = ecef.z;
                output["coordinate_system"] = "ecef";
            } else if (convertTo == "utm") {
                auto utm = converter.wgs84ToUTM(pt);
                output["easting"]  = utm.easting;
                output["northing"] = utm.northing;
                output["zone"]     = utm.zone;
                output["hemisphere"] = std::string(1, utm.hemisphere);
                output["coordinate_system"] = "utm";
            } else if (convertTo == "mercator") {
                auto merc = converter.wgs84ToMercator(pt);
                output["x"] = merc.x;
                output["y"] = merc.y;
                output["coordinate_system"] = "mercator";
            } else {
                throw std::invalid_argument("\u4e0d\u652f\u6301\u7684\u76ee\u6807\u5750\u6807\u7cfb: " + convertTo);
            }
        }

        if (bearingCmd->parsed()) {
            auto from = parsePoint2D(bearFromStr);
            auto to   = parsePoint2D(bearToStr);
            double result = bearing(from, to);
            output["bearing"]     = result;
            output["unit"]        = "deg";
        }

        if (visibleCmd->parsed()) {
            auto from = parsePoint3D(visFromStr);
            auto to   = parsePoint3D(visToStr);
            EarthConverter converter;
            bool result = converter.isVisible(from, to);
            output["is_visible"] = result;
        }

        if (inPolyCmd->parsed()) {
            auto point = parsePoint2D(polyPointStr);
            auto polyJson = json::parse(polyPolygonStr);
            std::vector<EarthPoint> polygon;
            for (const auto& pt : polyJson) {
                polygon.push_back(EarthPoint(pt[0].get<double>(), pt[1].get<double>()));
            }
            EarthGeometry::ProjectionType projection;
            if (polyProjType == "utm") {
                projection = EarthGeometry::ProjectionType::UTM;
            } else if (polyProjType == "mercator") {
                projection = EarthGeometry::ProjectionType::MERCATOR;
            } else {
                throw std::invalid_argument("\u4e0d\u652f\u6301\u7684\u6295\u5f71\u7c7b\u578b: " + polyProjType);
            }
            EarthGeometry geometry;
            bool result = geometry.isPointInPolygon(point, polygon, projection);
            output["is_inside"] = result;
            output["projection"] = polyProjType;
        }

        if (angleCmd->parsed()) {
            double result = 0.0;
            if (angleFrom == "deg" && angleTo == "rad") {
                result = degreesToRadians(angleValue);
            } else if (angleFrom == "rad" && angleTo == "deg") {
                result = radiansToDegrees(angleValue);
            } else if (angleFrom == angleTo) {
                result = angleValue;
            } else {
                throw std::invalid_argument("\u4e0d\u652f\u6301\u7684\u89d2\u5ea6\u5355\u4f4d: " + angleFrom + " -> " + angleTo);
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

        if (execPathCmd->parsed()) {
            output["executable_path"] = ExecPathUtils::getExecutablePath();
            output["executable_dir"]  = ExecPathUtils::getExecutableDir();
        }

        if (sysTypeCmd->parsed()) {
            output["system_type"] = ExecPathUtils::getSystemType();
        }

    } catch (const std::exception& e) {
        json err;
        err["error"] = e.what();
        std::cerr << err.dump(4) << std::endl;
        return 1;
    }

    if (!output.is_null()) {
        std::cout << output.dump(4) << std::endl;
    }

    return exitCode;
}