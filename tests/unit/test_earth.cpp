/**
 * @file test_earth.cpp
 * @brief 地球坐标模块单元测试
 * @author yAlgo Team
 * @date 2026-03-24
 */

#include <iostream>
#include <cmath>
#include "earth/earth_point.h"
#include "earth/earth_converter.h"

/**
 * @brief 测试地球点创建功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_earthPointCreation() {
    yalgo::earth::EarthPoint point(116.4, 39.9, 0.0);
    if (std::abs(point.longitude() - 116.4) > 1e-10 ||
        std::abs(point.latitude() - 39.9) > 1e-10) {
        std::cerr << "EarthPoint creation failed" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 测试坐标转换功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_coordinateConversion() {
    yalgo::earth::EarthPoint point(116.4, 39.9, 0.0); // 北京
    yalgo::earth::EarthConverter converter;
    
    // 测试 WGS84 到 UTM 转换
    yalgo::earth::EarthConverter::UTMCoordinate utm = converter.wgs84ToUTM(point);
    
    // 测试 UTM 回转到 WGS84
    yalgo::earth::EarthPoint recovered = converter.utmToWGS84(utm);
    
    // 检查误差（应在合理范围内）
    double lng_err = std::abs(point.longitude() - recovered.longitude());
    double lat_err = std::abs(point.latitude() - recovered.latitude());
    
    if (lng_err > 1e-6 || lat_err > 1e-6) {
        std::cerr << "Coordinate conversion error too large: lng_err=" << lng_err 
                  << ", lat_err=" << lat_err << std::endl;
        return false;
    }
    
    return true;
}

/**
 * @brief 测试墨卡托投影功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_mercatorProjection() {
    yalgo::earth::EarthPoint point(116.4, 39.9, 0.0);
    yalgo::earth::EarthConverter converter;
    
    // 测试 WGS84 到墨卡托转换
    yalgo::earth::EarthConverter::MercatorCoordinate merc = converter.wgs84ToMercator(point);
    
    // 测试墨卡托回转到 WGS84
    yalgo::earth::EarthPoint recovered = converter.mercatorToWGS84(merc);
    
    // 检查误差
    double lng_err = std::abs(point.longitude() - recovered.longitude());
    double lat_err = std::abs(point.latitude() - recovered.latitude());
    
    if (lng_err > 1e-6 || lat_err > 1e-6) {
        std::cerr << "Mercator projection error too large: lng_err=" << lng_err 
                  << ", lat_err=" << lat_err << std::endl;
        return false;
    }
    
    return true;
}

/**
 * @brief 主函数 - 运行所有地球坐标模块测试
 * @return 所有测试通过返回 0，否则返回 1
 */
int main() {
    std::cout << "Running earth module unit tests..." << std::endl;
    
    bool all_passed = true;
    
    all_passed &= test_earthPointCreation();
    all_passed &= test_coordinateConversion();
    all_passed &= test_mercatorProjection();
    
    if (all_passed) {
        std::cout << "All earth tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "Some earth tests failed!" << std::endl;
        return 1;
    }
}
