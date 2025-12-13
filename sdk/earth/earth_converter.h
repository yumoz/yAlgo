#pragma once

#include "earth_exports.h"
#include "earth_point.h"
#include <string>

namespace yalgo {
namespace earth {

/**
 * @brief 坐标系转换类
 * 
 * 实现不同坐标系之间的转换，支持以下转换：
 * - WGS84经纬度坐标 ↔ ECEF笛卡尔坐标
 * - WGS84经纬度坐标 ↔ UTM坐标
 * - 支持多种椭球体模型
 */
class EARTH_API EarthConverter {
public:
    /**
     * @brief 椭球体模型枚举
     */
    enum class Ellipsoid {
        WGS84,      ///< WGS84椭球体（默认）
        GRS80,      ///< GRS80椭球体
        CLARKE1866, ///< Clarke 1866椭球体
        AIRY,       ///< Airy椭球体
        BESSEL1841  ///< Bessel 1841椭球体
    };

    /**
     * @brief UTM坐标结构
     */
    struct UTMCoordinate {
        double easting;   ///< 东向坐标（米）
        double northing;  ///< 北向坐标（米）
        int zone;         ///< UTM投影带号
        char hemisphere;  ///< 半球（'N'或'S'）
        
        /**
         * @brief 将UTM坐标转换为字符串表示
         * 
         * @return std::string UTM坐标的字符串表示
         */
        EARTH_API std::string toString() const;
    };

    /**
     * @brief ECEF笛卡尔坐标结构
     */
    struct ECEFCoordinate {
        double x;  ///< X坐标（米）
        double y;  ///< Y坐标（米）
        double z;  ///< Z坐标（米）
        
        /**
         * @brief 将ECEF坐标转换为字符串表示
         * 
         * @return std::string ECEF坐标的字符串表示
         */
        EARTH_API std::string toString() const;
    };

    /**
     * @brief 默认构造函数
     * 
     * 使用WGS84椭球体模型
     */
    EarthConverter();

    /**
     * @brief 带椭球体参数的构造函数
     * 
     * @param ellipsoid 椭球体模型
     */
    explicit EarthConverter(Ellipsoid ellipsoid);

    /**
     * @brief 设置椭球体模型
     * 
     * @param ellipsoid 椭球体模型
     */
    void setEllipsoid(Ellipsoid ellipsoid);

    /**
     * @brief 获取当前椭球体模型
     * 
     * @return Ellipsoid 当前椭球体模型
     */
    Ellipsoid getEllipsoid() const;

    /**
     * @brief WGS84经纬度坐标转换为ECEF笛卡尔坐标
     * 
     * @param point 经纬度坐标点
     * @return ECEFCoordinate ECEF笛卡尔坐标
     */
    ECEFCoordinate wgs84ToECEF(const EarthPoint& point) const;

    /**
     * @brief ECEF笛卡尔坐标转换为WGS84经纬度坐标
     * 
     * @param ecef ECEF笛卡尔坐标
     * @return EarthPoint WGS84经纬度坐标点
     */
    EarthPoint ecefToWGS84(const ECEFCoordinate& ecef) const;

    /**
     * @brief WGS84经纬度坐标转换为UTM坐标
     * 
     * @param point 经纬度坐标点
     * @return UTMCoordinate UTM坐标
     */
    UTMCoordinate wgs84ToUTM(const EarthPoint& point) const;

    /**
     * @brief UTM坐标转换为WGS84经纬度坐标
     * 
     * @param utm UTM坐标
     * @return EarthPoint WGS84经纬度坐标点
     */
    EarthPoint utmToWGS84(const UTMCoordinate& utm) const;

    /**
     * @brief 获取椭球体的长半轴
     * 
     * @return double 长半轴（米）
     */
    double getSemiMajorAxis() const;

    /**
     * @brief 获取椭球体的扁率
     * 
     * @return double 扁率
     */
    double getFlattening() const;

    /**
     * @brief 获取椭球体的短半轴
     * 
     * @return double 短半轴（米）
     */
    double getSemiMinorAxis() const;

    /**
     * @brief 获取椭球体的第一偏心率平方
     * 
     * @return double 第一偏心率平方
     */
    double getEccentricitySquared() const;

    /**
     * @brief 获取椭球体的第二偏心率平方
     * 
     * @return double 第二偏心率平方
     */
    double getSecondEccentricitySquared() const;
    
    /**
     * @brief 判断地球上两点是否通视
     * 
     * 考虑地球曲率影响，判断两点之间是否存在视线路径
     * 
     * @param point1 第一个点
     * @param point2 第二个点
     * @return bool 如果两点通视返回true，否则返回false
     */
    bool isVisible(const EarthPoint& point1, const EarthPoint& point2) const;

private:
    /**
     * @brief 椭球体参数结构体
     */
    struct EllipsoidParams {
        double semiMajorAxis;      ///< 长半轴（米）
        double flattening;         ///< 扁率
        double semiMinorAxis;      ///< 短半轴（米）
        double eccentricitySquared; ///< 第一偏心率平方
        double secondEccentricitySquared; ///< 第二偏心率平方
    };

    /**
     * @brief 获取椭球体参数
     * 
     * @param ellipsoid 椭球体模型
     * @return EllipsoidParams 椭球体参数
     */
    EllipsoidParams getEllipsoidParams(Ellipsoid ellipsoid) const;

    Ellipsoid m_ellipsoid;        ///< 当前椭球体模型
    EllipsoidParams m_params;     ///< 当前椭球体参数
};

} // namespace earth
} // namespace yalgo
