#define _USE_MATH_DEFINES
#include "earth_point.h"
#include <sstream>
#include <iomanip>
#include <functional>

namespace yalgo {
namespace earth {

// 默认构造函数
EarthPoint::EarthPoint() : m_longitude(0.0), m_latitude(0.0), m_altitude(0.0) {
}

// 带参数构造函数
EarthPoint::EarthPoint(double longitude, double latitude, double altitude) 
    : m_longitude(longitude), m_latitude(latitude), m_altitude(altitude) {
}

// 拷贝构造函数
EarthPoint::EarthPoint(const EarthPoint& other) 
    : m_longitude(other.m_longitude), 
      m_latitude(other.m_latitude), 
      m_altitude(other.m_altitude) {
}

// 移动构造函数
EarthPoint::EarthPoint(EarthPoint&& other) noexcept 
    : m_longitude(other.m_longitude), 
      m_latitude(other.m_latitude), 
      m_altitude(other.m_altitude) {
    other.m_longitude = 0.0;
    other.m_latitude = 0.0;
    other.m_altitude = 0.0;
}

// 拷贝赋值运算符
EarthPoint& EarthPoint::operator=(const EarthPoint& other) {
    if (this != &other) {
        m_longitude = other.m_longitude;
        m_latitude = other.m_latitude;
        m_altitude = other.m_altitude;
    }
    return *this;
}

// 移动赋值运算符
EarthPoint& EarthPoint::operator=(EarthPoint&& other) noexcept {
    if (this != &other) {
        m_longitude = other.m_longitude;
        m_latitude = other.m_latitude;
        m_altitude = other.m_altitude;
        other.m_longitude = 0.0;
        other.m_latitude = 0.0;
        other.m_altitude = 0.0;
    }
    return *this;
}

// 加法运算符
EarthPoint EarthPoint::operator+(const EarthPoint& other) const {
    return EarthPoint(m_longitude + other.m_longitude, 
                     m_latitude + other.m_latitude, 
                     m_altitude + other.m_altitude);
}

// 减法运算符
EarthPoint EarthPoint::operator-(const EarthPoint& other) const {
    return EarthPoint(m_longitude - other.m_longitude, 
                     m_latitude - other.m_latitude, 
                     m_altitude - other.m_altitude);
}

// 乘法运算符
EarthPoint EarthPoint::operator*(double scalar) const {
    return EarthPoint(m_longitude * scalar, 
                     m_latitude * scalar, 
                     m_altitude * scalar);
}

// 除法运算符
EarthPoint EarthPoint::operator/(double scalar) const {
    if (scalar == 0.0) {
        // 处理除以零的情况
        return EarthPoint(0.0, 0.0, 0.0);
    }
    return EarthPoint(m_longitude / scalar, 
                     m_latitude / scalar, 
                     m_altitude / scalar);
}

// 加法赋值运算符
EarthPoint& EarthPoint::operator+=(const EarthPoint& other) {
    m_longitude += other.m_longitude;
    m_latitude += other.m_latitude;
    m_altitude += other.m_altitude;
    return *this;
}

// 减法赋值运算符
EarthPoint& EarthPoint::operator-=(const EarthPoint& other) {
    m_longitude -= other.m_longitude;
    m_latitude -= other.m_latitude;
    m_altitude -= other.m_altitude;
    return *this;
}

// 乘法赋值运算符
EarthPoint& EarthPoint::operator*=(double scalar) {
    m_longitude *= scalar;
    m_latitude *= scalar;
    m_altitude *= scalar;
    return *this;
}

// 除法赋值运算符
EarthPoint& EarthPoint::operator/=(double scalar) {
    if (scalar != 0.0) {
        m_longitude /= scalar;
        m_latitude /= scalar;
        m_altitude /= scalar;
    }
    return *this;
}

// 相等运算符
bool EarthPoint::operator==(const EarthPoint& other) const {
    const double epsilon = 1e-9;
    return std::abs(m_longitude - other.m_longitude) < epsilon &&
           std::abs(m_latitude - other.m_latitude) < epsilon &&
           std::abs(m_altitude - other.m_altitude) < epsilon;
}

// 不相等运算符
bool EarthPoint::operator!=(const EarthPoint& other) const {
    return !(*this == other);
}

// 获取经度
double EarthPoint::longitude() const {
    return m_longitude;
}

// 获取纬度
double EarthPoint::latitude() const {
    return m_latitude;
}

// 获取高度
double EarthPoint::altitude() const {
    return m_altitude;
}

// 设置经度
void EarthPoint::setLongitude(double longitude) {
    m_longitude = longitude;
}

// 设置纬度
void EarthPoint::setLatitude(double latitude) {
    m_latitude = latitude;
}

// 设置高度
void EarthPoint::setAltitude(double altitude) {
    m_altitude = altitude;
}

// 设置所有坐标值
void EarthPoint::set(double longitude, double latitude, double altitude) {
    m_longitude = longitude;
    m_latitude = latitude;
    m_altitude = altitude;
}

// 计算两点之间的距离（使用Haversine公式）
double EarthPoint::distanceTo(const EarthPoint& other) const {
    const double earthRadius = 6371000.0; // 地球半径（米）
    
    double lon1Rad = m_longitude * M_PI / 180.0;
    double lat1Rad = m_latitude * M_PI / 180.0;
    double lon2Rad = other.m_longitude * M_PI / 180.0;
    double lat2Rad = other.m_latitude * M_PI / 180.0;
    
    double dLat = lat2Rad - lat1Rad;
    double dLon = lon2Rad - lon1Rad;
    
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1Rad) * std::cos(lat2Rad) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    double distance = earthRadius * c;
    
    // 考虑高度差
    double heightDiff = m_altitude - other.m_altitude;
    distance = std::sqrt(distance * distance + heightDiff * heightDiff);
    
    return distance;
}

// 计算两点之间的方位角
double EarthPoint::bearingTo(const EarthPoint& other) const {
    double lon1Rad = m_longitude * M_PI / 180.0;
    double lat1Rad = m_latitude * M_PI / 180.0;
    double lon2Rad = other.m_longitude * M_PI / 180.0;
    double lat2Rad = other.m_latitude * M_PI / 180.0;
    
    double dLon = lon2Rad - lon1Rad;
    
    double y = std::sin(dLon) * std::cos(lat2Rad);
    double x = std::cos(lat1Rad) * std::sin(lat2Rad) -
               std::sin(lat1Rad) * std::cos(lat2Rad) * std::cos(dLon);
    
    double bearing = std::atan2(y, x) * 180.0 / M_PI;
    
    // 将方位角转换为0-360度
    if (bearing < 0) {
        bearing += 360.0;
    }
    
    return bearing;
}

// 将点转换为字符串表示
std::string EarthPoint::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    oss << "EarthPoint(longitude: " << m_longitude << ", latitude: " << m_latitude << ", altitude: " << m_altitude << ")";
    return oss.str();
}

// 获取点的哈希值
size_t EarthPoint::hash() const {
    std::hash<double> hasher;
    size_t seed = 0;
    
    // 使用异或和位移来组合哈希值
    seed ^= hasher(m_longitude) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hasher(m_latitude) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hasher(m_altitude) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    
    return seed;
}

// 计算两个点的点积
double dot(const EarthPoint& lhs, const EarthPoint& rhs) {
    return lhs.longitude() * rhs.longitude() +
           lhs.latitude() * rhs.latitude() +
           lhs.altitude() * rhs.altitude();
}

// 计算两个点的叉积
EarthPoint cross(const EarthPoint& lhs, const EarthPoint& rhs) {
    double x = lhs.latitude() * rhs.altitude() - lhs.altitude() * rhs.latitude();
    double y = lhs.altitude() * rhs.longitude() - lhs.longitude() * rhs.altitude();
    double z = lhs.longitude() * rhs.latitude() - lhs.latitude() * rhs.longitude();
    
    return EarthPoint(x, y, z);
}

// 计算点的长度
double length(const EarthPoint& point) {
    return std::sqrt(length2(point));
}

// 计算点的长度的平方
double length2(const EarthPoint& point) {
    return dot(point, point);
}

// 归一化点
EarthPoint normalize(const EarthPoint& point) {
    double len = length(point);
    if (len == 0.0) {
        return EarthPoint(0.0, 0.0, 0.0);
    }
    return point / len;
}

// 计算两点之间的距离
double distance(const EarthPoint& lhs, const EarthPoint& rhs) {
    return lhs.distanceTo(rhs);
}

// 计算两点之间的方位角
double bearing(const EarthPoint& from, const EarthPoint& to) {
    return from.bearingTo(to);
}

} // namespace earth
} // namespace yalgo
