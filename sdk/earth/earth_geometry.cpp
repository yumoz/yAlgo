#define _USE_MATH_DEFINES
#include "earth_geometry.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace yalgo {
namespace earth {

/**
 * @brief 判断点是否在多边形内
 * 
 * 使用射线法判断点是否在多边形内，支持UTM和墨卡托投影
 * 
 * @param point 要判断的点
 * @param polygon 多边形顶点集合
 * @param projectionType 投影类型
 * @return bool 如果点在多边形内返回true，否则返回false
 */
bool EarthGeometry::isPointInPolygon(const EarthPoint& point, const std::vector<EarthPoint>& polygon, ProjectionType projectionType) const {
    int n = polygon.size();
    if (n < 3) return false;
    
    EarthConverter converter;
    std::vector<EarthConverter::MercatorCoordinate> projPolygon;
    EarthConverter::MercatorCoordinate projPoint;
    
    // 将点和多边形顶点转换到指定投影坐标系
    if (projectionType == ProjectionType::UTM) {
        // 使用UTM投影
        projPoint = EarthConverter::MercatorCoordinate(
            converter.wgs84ToUTM(point).easting,
            converter.wgs84ToUTM(point).northing
        );
        
        for (const auto& polyPoint : polygon) {
            EarthConverter::UTMCoordinate utm = converter.wgs84ToUTM(polyPoint);
            projPolygon.emplace_back(utm.easting, utm.northing);
        }
    } else {
        // 使用墨卡托投影
        projPoint = converter.wgs84ToMercator(point);
        
        for (const auto& polyPoint : polygon) {
            projPolygon.push_back(converter.wgs84ToMercator(polyPoint));
        }
    }
    
    // 射线法判断点是否在多边形内
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        const EarthConverter::MercatorCoordinate& p1 = projPolygon[i];
        const EarthConverter::MercatorCoordinate& p2 = projPolygon[j];
        
        // 检查点是否在边上
        bool onEdge = (std::min(p1.y, p2.y) - 1e-3 <= projPoint.y && projPoint.y <= std::max(p1.y, p2.y) + 1e-3) &&
                     (std::min(p1.x, p2.x) - 1e-3 <= projPoint.x && projPoint.x <= std::max(p1.x, p2.x) + 1e-3) &&
                     (std::fabs((p2.x - p1.x) * (projPoint.y - p1.y) - (p2.y - p1.y) * (projPoint.x - p1.x)) < 1e-3);
        
        if (onEdge) return true;
        
        // 判断射线是否与边相交
        bool crossCond1 = (p1.y > projPoint.y) != (p2.y > projPoint.y);
        double crossX = ((projPoint.y - p1.y) * (p2.x - p1.x)) / (p2.y - p1.y) + p1.x;
        bool crossCond2 = projPoint.x < crossX - 1e-3;
        
        if (crossCond1 && crossCond2) {
            inside = !inside;
        }
    }
    
    return inside;
}

/**
 * @brief 计算投影精度统计
 * 
 * @param testPoints 测试点集合
 * @param projectionType 投影类型
 */
void EarthGeometry::calcProjectionAccuracy(const std::vector<EarthPoint>& testPoints, ProjectionType projectionType) const {
    if (testPoints.empty()) {
        std::cout << "精度统计：测试点集合为空！" << std::endl;
        return;
    }
    
    std::cout << "\n===== " << (projectionType == ProjectionType::UTM ? "UTM投影" : "墨卡托投影") << "精度统计结果 =====" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    
    double totalLngErr = 0.0;   // 总经度误差（度）
    double totalLatErr = 0.0;   // 总纬度误差（度）
    double totalDistErr = 0.0;  // 总距离误差（米）
    int validCount = 0;         // 有效测试点数
    
    EarthConverter converter;
    
    for (const auto& orig : testPoints) {
        // 跳过超范围点
        if (orig.latitude() < -85.05 || orig.latitude() > 85.05) {
            std::cout << "跳过无效点（纬度超范围）：" << orig.longitude() << "°, " << orig.latitude() << "°" << std::endl;
            continue;
        }
        
        EarthPoint recover;
        // 投影-反投影
        if (projectionType == ProjectionType::UTM) {
            EarthConverter::UTMCoordinate utm = converter.wgs84ToUTM(orig);
            recover = converter.utmToWGS84(utm);
        } else {
            EarthConverter::MercatorCoordinate merc = converter.wgs84ToMercator(orig);
            recover = converter.mercatorToWGS84(merc);
        }
        
        // 计算经纬度误差（度）
        double lngErr = std::fabs(orig.longitude() - recover.longitude());
        double latErr = std::fabs(orig.latitude() - recover.latitude());
        totalLngErr += lngErr;
        totalLatErr += latErr;
        
        // 计算地表距离误差（米，基于Haversine公式）
        double radOrigLng = orig.longitude() * M_PI / 180.0;
        double radOrigLat = orig.latitude() * M_PI / 180.0;
        double radRecLng = recover.longitude() * M_PI / 180.0;
        double radRecLat = recover.latitude() * M_PI / 180.0;
        
        double dLng = radRecLng - radOrigLng;
        double dLat = radRecLat - radOrigLat;
        double a = sin(dLat / 2) * sin(dLat / 2) + cos(radOrigLat) * cos(radRecLat) * sin(dLng / 2) * sin(dLng / 2);
        double distErr = converter.getSemiMajorAxis() * 2 * atan2(sqrt(a), sqrt(1 - a));
        totalDistErr += distErr;
        
        validCount++;
        // 打印单个点误差
        std::cout << "原始点：(" << orig.longitude() << ", " << orig.latitude() << ") → "
                  << "反算点：(" << recover.longitude() << ", " << recover.latitude() << ") → "
                  << "经差：" << lngErr << "°, 纬差：" << latErr << "°, 距离误差：" << distErr << "m" << std::endl;
    }
    
    // 统计平均值（仅对有效点）
    if (validCount > 0) {
        double avgLngErr = totalLngErr / validCount;
        double avgLatErr = totalLatErr / validCount;
        double avgDistErr = totalDistErr / validCount;
        
        std::cout << "------------------------" << std::endl;
        std::cout << "有效测试点数：" << validCount << std::endl;
        std::cout << "平均经度误差：" << avgLngErr << "°（≈" << avgLngErr * 111319.9 << "m）" << std::endl;
        std::cout << "平均纬度误差：" << avgLatErr << "°（≈" << avgLatErr * 111319.9 << "m）" << std::endl;
        std::cout << "平均距离误差：" << avgDistErr << "m" << std::endl;
        std::cout << "最大允许误差：<0.1m（卫星通信定位要求）" << std::endl;
    } else {
        std::cout << "无有效测试点，无法统计精度！" << std::endl;
    }
}

/**
 * @brief GEO卫星中国区域覆盖测试
 * 
 * 模拟GEO卫星（105°E赤道上空）的中国主覆盖区，测试典型城市是否在覆盖内
 */
void EarthGeometry::testGEOCoverageInChina() const {
    std::cout << "\n===== GEO卫星中国区域覆盖测试 =====" << std::endl;
    
    // 定义GEO卫星中国覆盖区闭合多边形
    std::vector<EarthPoint> geoChinaCoverage = {
        EarthPoint(73.5, 53.5),   // 漠河（最北）
        EarthPoint(135.0, 48.5),  // 抚远（最东）
        EarthPoint(122.0, 20.0),  // 三亚（最南）
        EarthPoint(73.5, 21.0),   // 西双版纳（最西）
        EarthPoint(73.5, 53.5)    // 闭合：与第一点重合
    };
    std::cout << "GEO覆盖区范围：漠河→抚远→三亚→西双版纳→漠河" << std::endl;
    
    // 定义测试城市
    std::vector<std::pair<EarthPoint, std::string>> testCities = {
        {EarthPoint(116.4, 39.9), "北京（覆盖内）"},
        {EarthPoint(121.4, 31.2), "上海（覆盖内）"},
        {EarthPoint(104.0, 30.6), "成都（覆盖内）"},
        {EarthPoint(113.2, 23.1), "广州（覆盖内）"},
        {EarthPoint(87.6, 43.8),  "乌鲁木齐（覆盖边界）"},
        {EarthPoint(110.0, 18.4), "海口（覆盖内）"},
        {EarthPoint(127.4, 43.8), "哈尔滨（覆盖内）"},
        {EarthPoint(100.5, 19.0), "西双版纳（覆盖内，顶点4）"},
        {EarthPoint(140.0, 35.0), "东京（日本，覆盖外）"},
        {EarthPoint(90.0, 60.0),  "西伯利亚（俄罗斯，覆盖外）"}
    };
    
    // 测试每个城市是否在GEO覆盖内（使用UTM投影）
    std::cout << "\n城市覆盖判断结果（UTM投影）：" << std::endl;
    for (const auto& city : testCities) {
        const EarthPoint& cityPoint = city.first;
        const std::string& cityName = city.second;
        bool isIn = isPointInPolygon(cityPoint, geoChinaCoverage, ProjectionType::UTM);
        std::cout << cityName << "：" << (isIn ? "✅ 在覆盖区内" : "❌ 在覆盖区外") 
                  << " → 坐标：(" << cityPoint.longitude() << "°, " << cityPoint.latitude() << "°)" << std::endl;
    }
    
    // 提取测试城市坐标用于精度统计
    std::vector<EarthPoint> cityPoints;
    for (const auto& city : testCities) {
        cityPoints.push_back(city.first);
    }
    
    // 进行投影精度统计
    calcProjectionAccuracy(cityPoints, ProjectionType::UTM);  // UTM精度统计
    calcProjectionAccuracy(cityPoints, ProjectionType::MERCATOR);  // 墨卡托精度统计
}

} // namespace earth
} // namespace yalgo