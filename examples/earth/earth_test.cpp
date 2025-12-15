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

// 演示直线距离计算
void EarthTest::demoEarthPointStraightLineDistance() {
    std::cout << "\n=== 直线距离计算 ===\n";
    
    // 北京（天安门）
    EarthPoint beijing(116.3974, 39.9093, 50.0);
    // 上海（东方明珠）
    EarthPoint shanghai(121.4999, 31.2397, 60.0);
    
    std::cout << "北京: " << beijing.toString() << std::endl;
    std::cout << "上海: " << shanghai.toString() << std::endl;
    
    // 计算直线距离
    double straightLineDist1 = beijing.straightLineDistanceTo(shanghai);
    double straightLineDist2 = straightLineDistance(beijing, shanghai);
    double sphericalDist = beijing.distanceTo(shanghai);
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "两点之间直线距离（成员函数）: " << straightLineDist1 << "米 = " << straightLineDist1 / 1000.0 << "公里" << std::endl;
    std::cout << "两点之间直线距离（全局函数）: " << straightLineDist2 << "米 = " << straightLineDist2 / 1000.0 << "公里" << std::endl;
    std::cout << "两点之间球面距离（Haversine）: " << sphericalDist << "米 = " << sphericalDist / 1000.0 << "公里" << std::endl;
    std::cout << "直线距离与球面距离之差: " << std::abs(straightLineDist1 - sphericalDist) << "米" << std::endl;
}

// 演示Vincenty球面距离计算
void EarthTest::demoEarthPointVincentyDistance() {
    std::cout << "\n=== Vincenty球面距离计算 ===\n";
    
    // 北京（天安门）
    EarthPoint beijing(116.3974, 39.9093, 50.0);
    // 上海（东方明珠）
    EarthPoint shanghai(121.4999, 31.2397, 60.0);
    
    std::cout << "北京: " << beijing.toString() << std::endl;
    std::cout << "上海: " << shanghai.toString() << std::endl;
    
    // 计算不同方法的球面距离
    double haversineDist = beijing.distanceTo(shanghai);
    double vincentyDist1 = beijing.vincentyDistanceTo(shanghai);
    double vincentyDist2 = vincentyDistance(beijing, shanghai);
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "两点之间球面距离（Haversine）: " << haversineDist << "米 = " << haversineDist / 1000.0 << "公里" << std::endl;
    std::cout << "两点之间球面距离（Vincenty，成员函数）: " << vincentyDist1 << "米 = " << vincentyDist1 / 1000.0 << "公里" << std::endl;
    std::cout << "两点之间球面距离（Vincenty，全局函数）: " << vincentyDist2 << "米 = " << vincentyDist2 / 1000.0 << "公里" << std::endl;
    std::cout << "Vincenty与Haversine距离之差: " << std::abs(vincentyDist1 - haversineDist) << "米" << std::endl;
}

// 演示坐标范围检查
void EarthTest::demoEarthPointCoordinateRange() {
    std::cout << "\n=== 坐标范围检查 ===\n";
    
    // 测试正常坐标
    EarthPoint normalPoint(116.3974, 39.9093, 50.0);
    std::cout << "正常坐标: " << normalPoint.toString() << std::endl;
    
    // 测试超出范围的坐标
    EarthPoint outOfRangePoint(200.0, 100.0, 100.0);
    std::cout << "超出范围坐标输入 (200.0, 100.0, 100.0) 后: " << outOfRangePoint.toString() << std::endl;
    
    // 测试负超出范围的坐标
    EarthPoint negativeOutOfRangePoint(-200.0, -100.0, 100.0);
    std::cout << "负超出范围坐标输入 (-200.0, -100.0, 100.0) 后: " << negativeOutOfRangePoint.toString() << std::endl;
    
    // 测试边界坐标
    EarthPoint boundaryPoint(180.0, 90.0, 0.0);
    std::cout << "边界坐标 (180.0, 90.0, 0.0): " << boundaryPoint.toString() << std::endl;
    
    // 测试边界外坐标
    EarthPoint beyondBoundaryPoint(181.0, 91.0, 0.0);
    std::cout << "边界外坐标输入 (181.0, 91.0, 0.0) 后: " << beyondBoundaryPoint.toString() << std::endl;
}

// 演示通视判断功能
void EarthTest::demoLineOfSight() {
    std::cout << "\n=== 通视判断 ===\n";
    
    EarthConverter converter;
    
    // 测试1: 两个近距离点（应该通视）
    EarthPoint pointA(116.3974, 39.9093, 50.0); // 北京天安门
    EarthPoint pointB(116.4074, 39.9093, 50.0); // 北京附近点，相距约1公里
    bool visible1 = converter.isVisible(pointA, pointB);
    
    std::cout << "点A: " << pointA.toString() << std::endl;
    std::cout << "点B: " << pointB.toString() << std::endl;
    std::cout << "两点是否通视: " << (visible1 ? "是" : "否") << std::endl;
    
    // 测试2: 两个远距离点（可能不通视）
    EarthPoint pointC(0.0, 0.0, 0.0); // 赤道上的点
    EarthPoint pointD(90.0, 0.0, 0.0); // 赤道上相距约1万公里的点
    bool visible2 = converter.isVisible(pointC, pointD);
    
    std::cout << "\n点C: " << pointC.toString() << std::endl;
    std::cout << "点D: " << pointD.toString() << std::endl;
    std::cout << "两点是否通视: " << (visible2 ? "是" : "否") << std::endl;
    
    // 测试3: 两个高点远距离点（可能通视）
    EarthPoint pointE(0.0, 0.0, 100000.0); // 赤道上100公里高空
    EarthPoint pointF(90.0, 0.0, 100000.0); // 赤道上100公里高空，相距约1万公里
    bool visible3 = converter.isVisible(pointE, pointF);
    
    std::cout << "\n点E: " << pointE.toString() << std::endl;
    std::cout << "点F: " << pointF.toString() << std::endl;
    std::cout << "两点是否通视: " << (visible3 ? "是" : "否") << std::endl;
}

// 演示WGS84与墨卡托坐标系转换
void EarthTest::demoWgs84ToMercatorConversion() {
    std::cout << "\n=== WGS84与墨卡托坐标系转换 ===\n";
    
    EarthConverter converter;
    
    // 北京（天安门）
    EarthPoint beijing(116.3974, 39.9093, 50.0);
    std::cout << "WGS84坐标: " << beijing.toString() << std::endl;
    
    // 转换为墨卡托坐标
    auto mercator = converter.wgs84ToMercator(beijing);
    std::cout << "墨卡托坐标: " << mercator.toString() << std::endl;
    
    // 转换回WGS84坐标
    auto backToWgs84 = converter.mercatorToWGS84(mercator);
    std::cout << "转换回WGS84: " << backToWgs84.toString() << std::endl;
    
    // 计算转换误差
    std::cout << std::fixed << std::setprecision(9);
    std::cout << "经度误差: " << std::abs(beijing.longitude() - backToWgs84.longitude()) << "度" << std::endl;
    std::cout << "纬度误差: " << std::abs(beijing.latitude() - backToWgs84.latitude()) << "度" << std::endl;
}

// 演示点在多边形内判断功能
void EarthTest::demoPointInPolygon() {
    std::cout << "\n=== 点在多边形内判断 ===\n";
    
    EarthGeometry geometry;
    
    // 创建一个简单的多边形（矩形）
    std::vector<EarthPoint> polygon = {
        EarthPoint(116.0, 39.0),
        EarthPoint(117.0, 39.0),
        EarthPoint(117.0, 40.0),
        EarthPoint(116.0, 40.0),
        EarthPoint(116.0, 39.0) // 闭合多边形
    };
    
    std::cout << "多边形顶点: " << std::endl;
    for (size_t i = 0; i < polygon.size(); i++) {
        std::cout << "  " << i + 1 << ": " << polygon[i].toString() << std::endl;
    }
    
    // 测试点
    std::vector<EarthPoint> testPoints = {
        EarthPoint(116.5, 39.5), // 多边形内部
        EarthPoint(116.2, 39.2), // 多边形内部
        EarthPoint(115.0, 39.0), // 多边形外部
        EarthPoint(116.5, 40.5), // 多边形外部
        EarthPoint(116.0, 39.5)  // 多边形边界上
    };
    
    // 使用UTM投影测试
    std::cout << "\nUTM投影下点在多边形内判断结果: " << std::endl;
    for (size_t i = 0; i < testPoints.size(); i++) {
        bool isIn = geometry.isPointInPolygon(testPoints[i], polygon, EarthGeometry::ProjectionType::UTM);
        std::cout << "  点" << i + 1 << " (" << testPoints[i].longitude() << ", " << testPoints[i].latitude() 
                  << "): " << (isIn ? "在多边形内" : "不在多边形内") << std::endl;
    }
    
    // 使用墨卡托投影测试
    std::cout << "\n墨卡托投影下点在多边形内判断结果: " << std::endl;
    for (size_t i = 0; i < testPoints.size(); i++) {
        bool isIn = geometry.isPointInPolygon(testPoints[i], polygon, EarthGeometry::ProjectionType::MERCATOR);
        std::cout << "  点" << i + 1 << " (" << testPoints[i].longitude() << ", " << testPoints[i].latitude() 
                  << "): " << (isIn ? "在多边形内" : "不在多边形内") << std::endl;
    }
}

// 演示GEO卫星中国区域覆盖测试
void EarthTest::demoGEOCoverageInChina() {
    std::cout << "\n=== GEO卫星中国区域覆盖测试 ===\n";
    
    EarthGeometry geometry;
    
    // 定义GEO卫星中国覆盖区闭合多边形（经纬度顶点，模拟105°E GEO卫星波束）
    std::vector<EarthPoint> geoChinaCoverage = {
        EarthPoint(73.5, 53.5),   // 顶点1：漠河（最北）
        EarthPoint(135.0, 48.5),  // 顶点2：抚远（最东）
        EarthPoint(122.0, 20.0),  // 顶点3：三亚（最南）
        EarthPoint(73.5, 21.0),   // 顶点4：西双版纳（最西）
        EarthPoint(73.5, 53.5)    // 闭合：与第一点重合
    };
    std::cout << "GEO覆盖区范围：漠河→抚远→三亚→西双版纳→漠河" << std::endl;
    
    // 定义测试城市（含覆盖内/外典型城市，覆盖边界城市）
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
        const EarthPoint& cityLatLng = city.first;
        const std::string& cityName = city.second;
        bool isIn = geometry.isPointInPolygon(cityLatLng, geoChinaCoverage, EarthGeometry::ProjectionType::UTM);
        std::cout << "  " << cityName << "：" << (isIn ? "✅ 在覆盖区内" : "❌ 在覆盖区外") 
                  << " → 坐标：(" << cityLatLng.longitude() << "°, " << cityLatLng.latitude() << "°)" << std::endl;
    }
    
    // 对测试城市进行投影精度统计
    std::vector<EarthPoint> cityPoints;
    for (const auto& city : testCities) {
        cityPoints.push_back(city.first);
    }
    
    std::cout << "\nUTM投影精度统计：" << std::endl;
    geometry.calcProjectionAccuracy(cityPoints, EarthGeometry::ProjectionType::UTM);
    
    std::cout << "\n墨卡托投影精度统计：" << std::endl;
    geometry.calcProjectionAccuracy(cityPoints, EarthGeometry::ProjectionType::MERCATOR);
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
    demoEarthPointStraightLineDistance();
    demoEarthPointVincentyDistance();
    demoEarthPointCoordinateRange();
    demoEarthPointBearing();
    demoVectorOperations();
    demoWgs84ToEcefConversion();
    demoWgs84ToUtmConversion();
    demoWgs84ToMercatorConversion();
    demoPointInPolygon();
    demoGEOCoverageInChina();
    demoEllipsoidModels();
    demoLineOfSight();
    
    std::cout << "\n========================================\n";
    std::cout << "所有地球坐标库测试完成\n";
    std::cout << "========================================\n";
}

} // namespace examples
} // namespace yalgo
