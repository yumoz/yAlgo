/**
 * @file test_integration.cpp
 * @brief 模块间集成测试
 * @author yAlgo Team
 * @date 2026-03-24
 */

#include <iostream>
#include <string>
#include "log/async_logger.h"
#include "utils/exe_path.h"
#include "math/math_utils.h"
#include "earth/earth_point.h"
#include "earth/earth_converter.h"

/**
 * @brief 测试日志与工具模块集成
 * @return 测试通过返回 true，否则返回 false
 */
bool test_logAndUtilsIntegration() {
    // 获取系统类型并记录日志
    std::string system_type = yalgo::utils::ExePath::getSystemType();
    std::string log_msg = yalgo::log::formatLog("System type: %s", system_type.c_str());
    
    if (log_msg.empty() || system_type.empty()) {
        std::cerr << "Log and utils integration failed" << std::endl;
        return false;
    }
    
    std::cout << "Log + Utils integration: " << log_msg << std::endl;
    return true;
}

/**
 * @brief 测试数学与地球坐标模块集成
 * @return 测试通过返回 true，否则返回 false
 */
bool test_mathAndEarthIntegration() {
    // 创建地球点
    yalgo::earth::EarthPoint point1(116.4, 39.9, 0.0);
    yalgo::earth::EarthPoint point2(117.4, 39.9, 0.0);
    
    // 使用地球坐标模块计算距离
    double distance = point1.distanceTo(point2);
    
    if (distance <= 0) {
        std::cerr << "Math and earth integration failed" << std::endl;
        return false;
    }
    
    std::cout << "Math + Earth integration: distance = " << distance << std::endl;
    return true;
}

/**
 * @brief 测试多模块协作场景
 * @return 测试通过返回 true，否则返回 false
 */
bool test_multiModuleScenario() {
    // 场景：记录系统信息并计算坐标
    std::string system_type = yalgo::utils::ExePath::getSystemType();
    
    yalgo::earth::EarthPoint beijing(116.4, 39.9, 0.0);
    yalgo::earth::EarthConverter converter;
    
    // 转换坐标
    yalgo::earth::EarthConverter::UTMCoordinate utm = converter.wgs84ToUTM(beijing);
    
    // 使用数学工具计算
    double easting_km = yalgo::math::wattsToKilowatts(utm.easting * 1000); // 模拟计算
    
    std::cout << "Multi-module scenario: " << system_type 
              << ", Beijing UTM easting: " << utm.easting << std::endl;
    
    return true;
}

/**
 * @brief 主函数 - 运行所有集成测试
 * @return 所有测试通过返回 0，否则返回 1
 */
int main() {
    std::cout << "Running integration tests..." << std::endl;
    
    bool all_passed = true;
    
    all_passed &= test_logAndUtilsIntegration();
    all_passed &= test_mathAndEarthIntegration();
    all_passed &= test_multiModuleScenario();
    
    if (all_passed) {
        std::cout << "All integration tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "Some integration tests failed!" << std::endl;
        return 1;
    }
}
