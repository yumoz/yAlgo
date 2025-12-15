#pragma once

#include "../../sdk/earth/earth_point.h"
#include "../../sdk/earth/earth_converter.h"
#include "../../sdk/earth/earth_geometry.h"
#include <vector>
#include <iostream>

namespace yalgo {
namespace examples {

/**
 * 地球坐标模块测试类
 * 封装所有地球坐标功能的测试方法
 */
class EarthTest {
public:
    /**
     * 演示EarthPoint构造函数和基本操作
     */
    static void demoEarthPointConstruction();
    
    /**
     * 演示EarthPoint运算符重载
     */
    static void demoEarthPointOperators();
    
    /**
     * 演示EarthPoint的距离计算功能
     */
    static void demoEarthPointDistance();
    
    /**
     * 演示EarthPoint的方位角计算功能
     */
    static void demoEarthPointBearing();
    
    /**
     * 演示向量操作（点积、叉积、归一化等）
     */
    static void demoVectorOperations();
    
    /**
     * 演示WGS84与ECEF坐标系转换
     */
    static void demoWgs84ToEcefConversion();
    
    /**
     * 演示WGS84与UTM坐标系转换
     */
    static void demoWgs84ToUtmConversion();
    
    /**
     * 演示不同椭球体模型的使用
     */
    static void demoEllipsoidModels();
    
    /**
     * 演示直线距离计算
     */
    static void demoEarthPointStraightLineDistance();
    
    /**
     * 演示Vincenty球面距离计算
     */
    static void demoEarthPointVincentyDistance();
    
    /**
     * 演示坐标范围检查
     */
    static void demoEarthPointCoordinateRange();
    
    /**
     * 演示通视判断功能
     */
    static void demoLineOfSight();
    
    /**
     * 演示WGS84与墨卡托坐标系转换
     */
    static void demoWgs84ToMercatorConversion();
    
    /**
     * 演示点在多边形内判断功能
     */
    static void demoPointInPolygon();
    
    /**
     * 演示GEO卫星中国区域覆盖测试
     */
    static void demoGEOCoverageInChina();
    
    /**
     * 运行所有测试
     */
    static void runAllTests();
};

} // namespace examples
} // namespace yalgo
