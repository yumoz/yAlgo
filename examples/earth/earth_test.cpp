#include "earth_test.h"
#include <iomanip>

namespace yalgo {
namespace examples {

using namespace yalgo::earth;

// 演示EarthPoint构造函数和基本操作
void EarthTest::demoEarthPointConstruction() {
    std::cout << "\n=== EarthPoint构造函数和基本操作 ===\n";
    
    // 默认构造函数
    EarthPoint point1;
    std::cout << "默认构造函数: " << point1.toString() << std::endl;
    
    // 带参数构造函数
    EarthPoint point2(116.3974, 39.9093, 50.0);
    std::cout << "带参数构造函数: " << point2.toString() << std::endl;
    
    // 拷贝构造函数
    EarthPoint point3(point2);
    std::cout << "拷贝构造函数: " << point3.toString() << std::endl;
    
    // 移动构造函数
    EarthPoint point4(std::move(point3));
    std::cout << "移动构造函数: " << point4.toString() << std::endl;
    std::cout << "移动后原对象: " << point3.toString() << std::endl;
    
    // 设置和获取坐标
    point1.setLongitude(120.1551);
    point1.setLatitude(30.2741);
    point1.setAltitude(100.0);
    std::cout << "设置坐标后: " << point1.toString() << std::endl;
    
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "经度: " << point1.longitude() << "度" << std::endl;
    std::cout << "纬度: " << point1.latitude() << "度" << std::endl;
    std::cout << "高度: " << point1.altitude() << "米" << std::endl;
}

// 演示EarthPoint运算符重载
void EarthTest::demoEarthPointOperators() {
    std::cout << "\n=== EarthPoint运算符重载 ===\n";
    
    EarthPoint p1(100.0, 30.0, 100.0);
    EarthPoint p2(20.0, 10.0, 50.0);
    
    std::cout << "p1: " << p1.toString() << std::endl;
    std::cout << "p2: " << p2.toString() << std::endl;
    
    // 加法运算符
    EarthPoint p3 = p1 + p2;
    std::cout << "p1 + p2: " << p3.toString() << std::endl;
    
    // 减法运算符
    EarthPoint p4 = p1 - p2;
    std::cout << "p1 - p2: " << p4.toString() << std::endl;
    
    // 乘法运算符
    EarthPoint p5 = p1 * 2.0;
    std::cout << "p1 * 2.0: " << p5.toString() << std::endl;
    
    // 除法运算符
    EarthPoint p6 = p1 / 2.0;
    std::cout << "p1 / 2.0: " << p6.toString() << std::endl;
    
    // 加法赋值运算符
    EarthPoint p7 = p1;
    p7 += p2;
    std::cout << "p1 += p2: " << p7.toString() << std::endl;
    
    // 减法赋值运算符
    EarthPoint p8 = p1;
    p8 -= p2;
    std::cout << "p1 -= p2: " << p8.toString() << std::endl;
    
    // 乘法赋值运算符
    EarthPoint p9 = p1;
    p9 *= 2.0;
    std::cout << "p1 *= 2.0: " << p9.toString() << std::endl;
    
    // 除法赋值运算符
    EarthPoint p10 = p1;
    p10 /= 2.0;
    std::cout << "p1 /= 2.0: " << p10.toString() << std::endl;
    
    // 相等运算符
    EarthPoint p11 = p1;
    std::cout << "p1 == p11: " << (p1 == p11 ? "true" : "false") << std::endl;
    std::cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << std::endl;
    
    // 不相等运算符
    std::cout << "p1 != p11: " << (p1 != p11 ? "true" : "false") << std::endl;
    std::cout << "p1 != p2: " << (p1 != p2 ? "true" : "false") << std::endl;
}

// 演示EarthPoint的距离计算功能
void EarthTest::demoEarthPointDistance() {
    std::cout << "\n=== EarthPoint距离计算 ===\n";
    
    // 北京（天安门）
    EarthPoint beijing(116.3974, 39.9093, 50.0);
    // 上海（东方明珠）
    EarthPoint shanghai(121.4999, 31.2397, 60.0);
    
    std::cout << "北京: " << beijing.toString() << std::endl;
    std::cout << "上海: " << shanghai.toString() << std::endl;
    
    // 计算两点之间的距离
    double distance1 = beijing.distanceTo(shanghai);
    double distance2 = distance(beijing, shanghai);
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "两点之间距离（成员函数）: " << distance1 << "米 = " << distance1 / 1000.0 << "公里" << std::endl;
    std::cout << "两点之间距离（全局函数）: " << distance2 << "米 = " << distance2 / 1000.0 << "公里" << std::endl;
}

// 演示EarthPoint的方位角计算功能
void EarthTest::demoEarthPointBearing() {
    std::cout << "\n=== EarthPoint方位角计算 ===\n";
    
    // 北京（天安门）
    EarthPoint beijing(116.3974, 39.9093, 50.0);
    // 上海（东方明珠）
    EarthPoint shanghai(121.4999, 31.2397, 60.0);
    // 广州（广州塔）
    EarthPoint guangzhou(113.3300, 23.1291, 100.0);
    
    std::cout << "北京: " << beijing.toString() << std::endl;
    std::cout << "上海: " << shanghai.toString() << std::endl;
    std::cout << "广州: " << guangzhou.toString() << std::endl;
    
    // 计算方位角
    double bearing1 = beijing.bearingTo(shanghai);
    double bearing2 = bearing(beijing, guangzhou);
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "北京到上海的方位角: " << bearing1 << "度" << std::endl;
    std::cout << "北京到广州的方位角: " << bearing2 << "度" << std::endl;
}

// 演示向量操作（点积、叉积、归一化等）
void EarthTest::demoVectorOperations() {
    std::cout << "\n=== 向量操作 ===\n";
    
    EarthPoint p1(1.0, 2.0, 3.0);
    EarthPoint p2(4.0, 5.0, 6.0);
    
    std::cout << "p1: " << p1.toString() << std::endl;
    std::cout << "p2: " << p2.toString() << std::endl;
    
    // 点积
    double dotProduct = dot(p1, p2);
    std::cout << "点积 (p1·p2): " << dotProduct << std::endl;
    
    // 叉积
    EarthPoint crossProduct = cross(p1, p2);
    std::cout << "叉积 (p1×p2): " << crossProduct.toString() << std::endl;
    
    // 长度
    double length1 = length(p1);
    double length2 = length(p2);
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "p1的长度: " << length1 << std::endl;
    std::cout << "p2的长度: " << length2 << std::endl;
    
    // 长度的平方
    double length2_1 = dot(p1, p1);
    double length2_2 = dot(p2, p2);
    std::cout << "p1的长度平方: " << length2_1 << std::endl;
    std::cout << "p2的长度平方: " << length2_2 << std::endl;
    
    // 归一化
    EarthPoint norm1 = normalize(p1);
    EarthPoint norm2 = normalize(p2);
    std::cout << "p1归一化: " << norm1.toString() << std::endl;
    std::cout << "p2归一化: " << norm2.toString() << std::endl;
    
    // 验证归一化后的长度
    double normLength1 = length(norm1);
    double normLength2 = length(norm2);
    std::cout << "归一化后p1长度: " << normLength1 << std::endl;
    std::cout << "归一化后p2长度: " << normLength2 << std::endl;
}

// 演示WGS84与ECEF坐标系转换
void EarthTest::demoWgs84ToEcefConversion() {
    std::cout << "\n=== WGS84与ECEF坐标系转换 ===\n";
    
    EarthConverter converter;
    
    // 北京（天安门）
    EarthPoint beijing(116.3974, 39.9093, 50.0);
    std::cout << "WGS84坐标: " << beijing.toString() << std::endl;
    
    // 转换为ECEF坐标
    auto ecef = converter.wgs84ToECEF(beijing);
    std::cout << "ECEF坐标: " << ecef.toString() << std::endl;
    
    // 转换回WGS84坐标
    auto backToWgs84 = converter.ecefToWGS84(ecef);
    std::cout << "转换回WGS84: " << backToWgs84.toString() << std::endl;
    
    // 计算转换误差
    std::cout << std::fixed << std::setprecision(9);
    std::cout << "经度误差: " << std::abs(beijing.longitude() - backToWgs84.longitude()) << "度" << std::endl;
    std::cout << "纬度误差: " << std::abs(beijing.latitude() - backToWgs84.latitude()) << "度" << std::endl;
    std::cout << "高度误差: " << std::abs(beijing.altitude() - backToWgs84.altitude()) << "米" << std::endl;
}

// 演示WGS84与UTM坐标系转换
void EarthTest::demoWgs84ToUtmConversion() {
    std::cout << "\n=== WGS84与UTM坐标系转换 ===\n";
    
    EarthConverter converter;
    
    // 北京（天安门）
    EarthPoint beijing(116.3974, 39.9093, 50.0);
    std::cout << "WGS84坐标: " << beijing.toString() << std::endl;
    
    // 转换为UTM坐标
    auto utm = converter.wgs84ToUTM(beijing);
    std::cout << "UTM坐标: " << utm.toString() << std::endl;
    
    // 转换回WGS84坐标
    auto backToWgs84 = converter.utmToWGS84(utm);
    std::cout << "转换回WGS84: " << backToWgs84.toString() << std::endl;
    
    // 计算转换误差
    std::cout << std::fixed << std::setprecision(9);
    std::cout << "经度误差: " << std::abs(beijing.longitude() - backToWgs84.longitude()) << "度" << std::endl;
    std::cout << "纬度误差: " << std::abs(beijing.latitude() - backToWgs84.latitude()) << "度" << std::endl;
    
    // 测试南半球坐标
    EarthPoint sydney(151.2093, -33.8688, 30.0);
    std::cout << "\n悉尼WGS84坐标: " << sydney.toString() << std::endl;
    auto sydneyUtm = converter.wgs84ToUTM(sydney);
    std::cout << "悉尼UTM坐标: " << sydneyUtm.toString() << std::endl;
    auto backToSydney = converter.utmToWGS84(sydneyUtm);
    std::cout << "转换回WGS84: " << backToSydney.toString() << std::endl;
}

// 演示不同椭球体模型的使用
void EarthTest::demoEllipsoidModels() {
    std::cout << "\n=== 不同椭球体模型的使用 ===\n";
    
    // 测试点
    EarthPoint beijing(116.3974, 39.9093, 50.0);
    
    // 测试不同椭球体模型
    EarthConverter wgs84Converter(EarthConverter::Ellipsoid::WGS84);
    EarthConverter grs80Converter(EarthConverter::Ellipsoid::GRS80);
    EarthConverter clarkeConverter(EarthConverter::Ellipsoid::CLARKE1866);
    
    // 获取椭球体参数
    std::cout << std::fixed << std::setprecision(6);
    
    std::cout << "WGS84椭球体: " << std::endl;
    std::cout << "  长半轴: " << wgs84Converter.getSemiMajorAxis() << "米" << std::endl;
    std::cout << "  短半轴: " << wgs84Converter.getSemiMinorAxis() << "米" << std::endl;
    std::cout << "  扁率: " << wgs84Converter.getFlattening() << std::endl;
    
    std::cout << "GRS80椭球体: " << std::endl;
    std::cout << "  长半轴: " << grs80Converter.getSemiMajorAxis() << "米" << std::endl;
    std::cout << "  短半轴: " << grs80Converter.getSemiMinorAxis() << "米" << std::endl;
    std::cout << "  扁率: " << grs80Converter.getFlattening() << std::endl;
    
    // 转换为ECEF坐标并比较不同椭球体的结果
    auto ecefWgs84 = wgs84Converter.wgs84ToECEF(beijing);
    auto ecefGrs80 = grs80Converter.wgs84ToECEF(beijing);
    auto ecefClarke = clarkeConverter.wgs84ToECEF(beijing);
    
    std::cout << "\n不同椭球体下ECEF坐标: " << std::endl;
    std::cout << "WGS84: " << ecefWgs84.toString() << std::endl;
    std::cout << "GRS80: " << ecefGrs80.toString() << std::endl;
    std::cout << "CLARKE1866: " << ecefClarke.toString() << std::endl;
}

// 运行所有测试
void EarthTest::runAllTests() {
    std::cout << "========================================\n";
    std::cout << "yAlgo 地球坐标库测试\n";
    std::cout << "========================================\n";
    
    // 运行所有测试方法
    demoEarthPointConstruction();
    demoEarthPointOperators();
    demoEarthPointDistance();
    demoEarthPointBearing();
    demoVectorOperations();
    demoWgs84ToEcefConversion();
    demoWgs84ToUtmConversion();
    demoEllipsoidModels();
    
    std::cout << "\n========================================\n";
    std::cout << "所有地球坐标库测试完成\n";
    std::cout << "========================================\n";
}

} // namespace examples
} // namespace yalgo
