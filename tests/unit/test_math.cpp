/**
 * @file test_math.cpp
 * @brief 数学模块单元测试
 * @author yAlgo Team
 * @date 2026-03-24
 */

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <iostream>
#include <cmath>
#include "math/math_utils.h"

/**
 * @brief 测试角度转弧度功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_degreesToRadians() {
    double radians = yalgo::math::degreesToRadians(180.0);
    double expected = M_PI;
    if (std::abs(radians - expected) > 1e-10) {
        std::cerr << "degreesToRadians(180) = " << radians << ", expected " << expected << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 测试弧度转角度功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_radiansToDegrees() {
    double degrees = yalgo::math::radiansToDegrees(M_PI);
    double expected = 180.0;
    if (std::abs(degrees - expected) > 1e-10) {
        std::cerr << "radiansToDegrees(PI) = " << degrees << ", expected " << expected << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 测试瓦特转千瓦功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_wattsToKilowatts() {
    double kw = yalgo::math::wattsToKilowatts(1000.0);
    double expected = 1.0;
    if (std::abs(kw - expected) > 1e-10) {
        std::cerr << "wattsToKilowatts(1000) = " << kw << ", expected " << expected << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 测试分贝转换功能
 * @return 测试通过返回 true，否则返回 false
 */
bool test_dbConversions() {
    double dbm = yalgo::math::wattsToDbm(1.0);
    double expected = 30.0; // 1W = 30dBm
    if (std::abs(dbm - expected) > 0.1) {
        std::cerr << "wattsToDbm(1) = " << dbm << ", expected " << expected << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 主函数 - 运行所有数学模块测试
 * @return 所有测试通过返回 0，否则返回 1
 */
int main() {
    std::cout << "Running math module unit tests..." << std::endl;
    
    bool all_passed = true;
    
    all_passed &= test_degreesToRadians();
    all_passed &= test_radiansToDegrees();
    all_passed &= test_wattsToKilowatts();
    all_passed &= test_dbConversions();
    
    if (all_passed) {
        std::cout << "All math tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "Some math tests failed!" << std::endl;
        return 1;
    }
}
