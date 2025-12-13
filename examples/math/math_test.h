#define _USE_MATH_DEFINES

#ifndef YALGO_EXAMPLES_MATH_MATH_TEST_H
#define YALGO_EXAMPLES_MATH_MATH_TEST_H

#include "../../sdk/math/math_utils.h"
#include <iostream>

namespace yalgo {
namespace examples {

/**
 * 数学模块测试类
 * 封装所有数学工具功能的测试方法
 */
class MathTest {
public:
    /**
     * 演示角度到弧度转换
     */
    static void demoDegreesToRadians();
    
    /**
     * 演示弧度到角度转换
     */
    static void demoRadiansToDegrees();
    
    /**
     * 演示功率单位转换（瓦特到千瓦）
     */
    static void demoWattsToKilowatts();
    
    /**
     * 演示功率单位转换（千瓦到瓦特）
     */
    static void demoKilowattsToWatts();
    
    /**
     * 演示功率单位转换（瓦特到马力）
     */
    static void demoWattsToHorsepower();
    
    /**
     * 演示功率单位转换（马力到瓦特）
     */
    static void demoHorsepowerToWatts();
    
    /**
     * 演示瓦特到dBW转换
     */
    static void demoWattsToDbW();
    
    /**
     * 演示dBW到瓦特转换
     */
    static void demoDbWToWatts();
    
    /**
     * 演示瓦特到dBm转换
     */
    static void demoWattsToDbm();
    
    /**
     * 演示dBm到瓦特转换
     */
    static void demoDbmToWatts();
    
    /**
     * 演示dBW和dBm之间的转换
     */
    static void demoDbWAndDbmConversion();
    
    /**
     * 演示通用分贝转换
     */
    static void demoGeneralDbConversion();
    
    /**
     * 运行所有测试
     */
    static void runAllTests();
};

} // namespace examples
} // namespace yalgo

#endif // YALGO_EXAMPLES_MATH_MATH_TEST_H
