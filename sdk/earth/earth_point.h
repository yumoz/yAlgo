#pragma once

#include "earth_exports.h"
#include <string>
#include <cmath>

namespace yalgo {
namespace earth {

/**
 * @brief 经纬高坐标点类
 * 
 * 表示地球表面或空间中的一个点，使用经度、纬度和高度来描述位置。
 * 经度和纬度单位为度，高度单位为米。
 */
class EARTH_API EarthPoint {
public:
    /**
     * @brief 默认构造函数
     * 
     * 创建一个位于原点(0, 0, 0)的点
     */
    EarthPoint();

    /**
     * @brief 带参数构造函数
     * 
     * @param longitude 经度（度）
     * @param latitude 纬度（度）
     * @param altitude 高度（米）
     */
    EarthPoint(double longitude, double latitude, double altitude = 0.0);

    /**
     * @brief 拷贝构造函数
     * 
     * @param other 要拷贝的点
     */
    EarthPoint(const EarthPoint& other);

    /**
     * @brief 移动构造函数
     * 
     * @param other 要移动的点
     */
    EarthPoint(EarthPoint&& other) noexcept;

    /**
     * @brief 拷贝赋值运算符
     * 
     * @param other 要拷贝的点
     * @return EarthPoint& 自身引用
     */
    EarthPoint& operator=(const EarthPoint& other);

    /**
     * @brief 移动赋值运算符
     * 
     * @param other 要移动的点
     * @return EarthPoint& 自身引用
     */
    EarthPoint& operator=(EarthPoint&& other) noexcept;

    /**
     * @brief 加法运算符
     * 
     * @param other 要相加的点
     * @return EarthPoint 结果点
     */
    EarthPoint operator+(const EarthPoint& other) const;

    /**
     * @brief 减法运算符
     * 
     * @param other 要相减的点
     * @return EarthPoint 结果点
     */
    EarthPoint operator-(const EarthPoint& other) const;

    /**
     * @brief 乘法运算符
     * 
     * @param scalar 标量值
     * @return EarthPoint 结果点
     */
    EarthPoint operator*(double scalar) const;

    /**
     * @brief 除法运算符
     * 
     * @param scalar 标量值
     * @return EarthPoint 结果点
     */
    EarthPoint operator/(double scalar) const;

    /**
     * @brief 加法赋值运算符
     * 
     * @param other 要相加的点
     * @return EarthPoint& 自身引用
     */
    EarthPoint& operator+=(const EarthPoint& other);

    /**
     * @brief 减法赋值运算符
     * 
     * @param other 要相减的点
     * @return EarthPoint& 自身引用
     */
    EarthPoint& operator-=(const EarthPoint& other);

    /**
     * @brief 乘法赋值运算符
     * 
     * @param scalar 标量值
     * @return EarthPoint& 自身引用
     */
    EarthPoint& operator*=(double scalar);

    /**
     * @brief 除法赋值运算符
     * 
     * @param scalar 标量值
     * @return EarthPoint& 自身引用
     */
    EarthPoint& operator/=(double scalar);

    /**
     * @brief 相等运算符
     * 
     * @param other 要比较的点
     * @return bool 是否相等
     */
    bool operator==(const EarthPoint& other) const;

    /**
     * @brief 不相等运算符
     * 
     * @param other 要比较的点
     * @return bool 是否不相等
     */
    bool operator!=(const EarthPoint& other) const;

    /**
     * @brief 获取经度
     * 
     * @return double 经度（度）
     */
    double longitude() const;

    /**
     * @brief 获取纬度
     * 
     * @return double 纬度（度）
     */
    double latitude() const;

    /**
     * @brief 获取高度
     * 
     * @return double 高度（米）
     */
    double altitude() const;

    /**
     * @brief 设置经度
     * 
     * @param longitude 经度（度）
     */
    void setLongitude(double longitude);

    /**
     * @brief 设置纬度
     * 
     * @param latitude 纬度（度）
     */
    void setLatitude(double latitude);

    /**
     * @brief 设置高度
     * 
     * @param altitude 高度（米）
     */
    void setAltitude(double altitude);

    /**
     * @brief 设置所有坐标值
     * 
     * @param longitude 经度（度）
     * @param latitude 纬度（度）
     * @param altitude 高度（米）
     */
    void set(double longitude, double latitude, double altitude = 0.0);

    /**
     * @brief 计算两点之间的球面距离（使用Haversine公式）
     * 
     * @param other 另一个点
     * @return double 两点之间的球面距离（米）
     */
    double distanceTo(const EarthPoint& other) const;
    
    /**
     * @brief 计算两点之间的直线距离（欧几里得距离）
     * 
     * @param other 另一个点
     * @return double 两点之间的直线距离（米）
     */
    double straightLineDistanceTo(const EarthPoint& other) const;
    
    /**
     * @brief 计算两点之间的精确球面距离（使用Vincenty公式）
     * 
     * @param other 另一个点
     * @return double 两点之间的精确球面距离（米）
     */
    double vincentyDistanceTo(const EarthPoint& other) const;

    /**
     * @brief 计算两点之间的方位角
     * 
     * @param other 另一个点
     * @return double 方位角（度，0-360）
     */
    double bearingTo(const EarthPoint& other) const;

    /**
     * @brief 将点转换为字符串表示
     * 
     * @return std::string 点的字符串表示
     */
    std::string toString() const;

    /**
     * @brief 获取点的哈希值
     * 
     * @return size_t 哈希值
     */
    size_t hash() const;

private:
    double m_longitude;   ///< 经度（度）
    double m_latitude;    ///< 纬度（度）
    double m_altitude;    ///< 高度（米）
};

/**
 * @brief 计算两个点的点积
 * 
 * @param lhs 第一个点
 * @param rhs 第二个点
 * @return double 点积结果
 */
EARTH_API double dot(const EarthPoint& lhs, const EarthPoint& rhs);

/**
 * @brief 计算两个点的叉积
 * 
 * @param lhs 第一个点
 * @param rhs 第二个点
 * @return EarthPoint 叉积结果
 */
EARTH_API EarthPoint cross(const EarthPoint& lhs, const EarthPoint& rhs);

/**
 * @brief 计算点的长度
 * 
 * @param point 输入点
 * @return double 长度
 */
EARTH_API double length(const EarthPoint& point);

/**
 * @brief 计算点的长度的平方
 * 
 * @param point 输入点
 * @return double 长度的平方
 */
EARTH_API double length2(const EarthPoint& point);

/**
 * @brief 归一化点
 * 
 * @param point 输入点
 * @return EarthPoint 归一化后的点
 */
EARTH_API EarthPoint normalize(const EarthPoint& point);

/**
 * @brief 计算两点之间的球面距离（使用Haversine公式）
 * 
 * @param lhs 第一个点
 * @param rhs 第二个点
 * @return double 球面距离（米）
 */
EARTH_API double distance(const EarthPoint& lhs, const EarthPoint& rhs);

/**
 * @brief 计算两点之间的直线距离（欧几里得距离）
 * 
 * @param lhs 第一个点
 * @param rhs 第二个点
 * @return double 直线距离（米）
 */
EARTH_API double straightLineDistance(const EarthPoint& lhs, const EarthPoint& rhs);

/**
 * @brief 计算两点之间的精确球面距离（使用Vincenty公式）
 * 
 * @param lhs 第一个点
 * @param rhs 第二个点
 * @return double 精确球面距离（米）
 */
EARTH_API double vincentyDistance(const EarthPoint& lhs, const EarthPoint& rhs);

/**
 * @brief 计算两点之间的方位角
 * 
 * @param from 起点
 * @param to 终点
 * @return double 方位角（度，0-360）
 */
EARTH_API double bearing(const EarthPoint& from, const EarthPoint& to);

} // namespace earth
} // namespace yalgo

// 为EarthPoint类提供哈希支持
namespace std {
    template<>
    struct hash<yalgo::earth::EarthPoint> {
        size_t operator()(const yalgo::earth::EarthPoint& point) const {
            return point.hash();
        }
    };
}