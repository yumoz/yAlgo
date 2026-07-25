#include "earth_commands.h"
#include "cli_utils.h"
#include "../../3rd/cli/json.hpp"
#include "../../sdk/earth/earth_point.h"
#include "../../sdk/earth/earth_converter.h"
#include "../../sdk/earth/earth_geometry.h"

using namespace yalgo::earth;
using json = nlohmann::json;

static CLI::App* earthCmd = nullptr;
static CLI::App* distanceCmd = nullptr;
static CLI::App* convertCmd = nullptr;
static CLI::App* bearingCmd = nullptr;
static CLI::App* visibleCmd = nullptr;
static CLI::App* inPolyCmd = nullptr;

static std::string distFromStr, distToStr, distType = "haversine";
static std::string convertInputStr, convertFrom = "wgs84", convertTo;
static std::string bearFromStr, bearToStr;
static std::string visFromStr, visToStr;
static std::string polyPointStr, polyPolygonStr, polyProjType = "utm";

CLI::App* registerEarthCommands(CLI::App& app) {
    earthCmd = app.add_subcommand("earth", "地球坐标相关操作");
    earthCmd->require_subcommand();

    distanceCmd = earthCmd->add_subcommand("distance", "计算两点距离");
    distanceCmd->add_option("--from", distFromStr, "经纬度, 如 116.4,39.9")->required();
    distanceCmd->add_option("--to", distToStr, "经纬度, 如 121.5,31.2")->required();
    distanceCmd->add_option("--type", distType, "haversine|vincenty|straight, 默认 haversine");

    convertCmd = earthCmd->add_subcommand("convert", "坐标系转换");
    convertCmd->add_option("--input", convertInputStr, "经度,纬度,高度")->required();
    convertCmd->add_option("--from", convertFrom, "wgs84, 默认");
    convertCmd->add_option("--to", convertTo, "ecef|utm|mercator")->required();

    bearingCmd = earthCmd->add_subcommand("bearing", "计算方位角");
    bearingCmd->add_option("--from", bearFromStr, "经纬度, 如 116.4,39.9")->required();
    bearingCmd->add_option("--to", bearToStr, "经纬度, 如 121.5,31.2")->required();

    visibleCmd = earthCmd->add_subcommand("is-visible", "通视判断");
    visibleCmd->add_option("--from", visFromStr, "经度,纬度,高度")->required();
    visibleCmd->add_option("--to", visToStr, "经度,纬度,高度")->required();

    inPolyCmd = earthCmd->add_subcommand("is-point-in-polygon", "点在多边形内判断");
    inPolyCmd->add_option("--point", polyPointStr, "经纬度, 如 116.4,39.9")->required();
    inPolyCmd->add_option("--polygon", polyPolygonStr, "JSON数组 [[lng,lat],...]")->required();
    inPolyCmd->add_option("--projection", polyProjType, "utm|mercator");

    return earthCmd;
}

int executeEarthCommand() {
    json output;

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
            throw std::invalid_argument("不支持的距离类型: " + distType);
        }
        output["distance"] = result;
        output["unit"]     = "m";
        output["method"]   = distType;
    }

    if (convertCmd->parsed()) {
        if (convertFrom != "wgs84") {
            throw std::invalid_argument("仅支持从 wgs84 转换");
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
            throw std::invalid_argument("不支持的目标坐标系: " + convertTo);
        }
    }

    if (bearingCmd->parsed()) {
        auto from = parsePoint2D(bearFromStr);
        auto to   = parsePoint2D(bearToStr);
        double result = bearing(from, to);
        output["bearing"] = result;
        output["unit"]    = "deg";
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
            throw std::invalid_argument("不支持的投影类型: " + polyProjType);
        }
        EarthGeometry geometry;
        bool result = geometry.isPointInPolygon(point, polygon, projection);
        output["is_inside"] = result;
        output["projection"] = polyProjType;
    }

    if (!output.is_null()) {
        std::cout << output.dump(4) << std::endl;
    }
    return 0;
}
