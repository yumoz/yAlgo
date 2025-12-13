#define _USE_MATH_DEFINES
#include "earth_point.h"
#include <sstream>
#include <iomanip>
#include <functional>

namespace yalgo {
namespace earth {

// 辅助函数：规范化经度到[-180, 180]度
inline double normalizeLongitude(double longitude) {
    longitude = fmod(longitude + 180.0, 360.0);
    if (longitude < 0) {
        longitude += 360.0;
    }
    return longitude - 180.0;
}

// 辅助函数：规范化纬度到[-90, 90]度
inline double normalizeLatitude(double latitude) {
    if (latitude > 90.0) {
        latitude = 180.0 - latitude;
    } else if (latitude < -90.0) {
        latitude = -180.0 - latitude;
    }
    return latitude;
}

// 默认构造函数
EarthPoint::EarthPoint() : m_longitude(0.0), m_latitude(0.0), m_altitude(0.0) {
}

// 带参数构造函数
EarthPoint::EarthPoint(double longitude, double latitude, double altitude) 
    : m_longitude(normalizeLongitude(longitude)), 
      m_latitude(normalizeLatitude(latitude)), 
      m_altitude(altitude) {
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
    m_longitude = normalizeLongitude(longitude);
}

// 设置纬度
void EarthPoint::setLatitude(double latitude) {
    m_latitude = normalizeLatitude(latitude);
}

// 设置高度
void EarthPoint::setAltitude(double altitude) {
    m_altitude = altitude;
}

// 设置所有坐标值
void EarthPoint::set(double longitude, double latitude, double altitude) {
    m_longitude = normalizeLongitude(longitude);
    m_latitude = normalizeLatitude(latitude);
    m_altitude = altitude;
}

// 计算两点之间的球面距离（使用Haversine公式）
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

// 计算两点之间的直线距离（欧几里得距离）
double EarthPoint::straightLineDistanceTo(const EarthPoint& other) const {
    // 将经纬度转换为弧度
    double lon1Rad = m_longitude * M_PI / 180.0;
    double lat1Rad = m_latitude * M_PI / 180.0;
    double lon2Rad = other.m_longitude * M_PI / 180.0;
    double lat2Rad = other.m_latitude * M_PI / 180.0;
    
    // 地球半径（米）
    const double earthRadius = 6371000.0;
    
    // 将经纬度转换为近似的平面坐标（米）
    double x1 = earthRadius * cos(lat1Rad) * cos(lon1Rad) + m_altitude * cos(lat1Rad) * cos(lon1Rad);
    double y1 = earthRadius * cos(lat1Rad) * sin(lon1Rad) + m_altitude * cos(lat1Rad) * sin(lon1Rad);
    double z1 = earthRadius * sin(lat1Rad) + m_altitude * sin(lat1Rad);
    
    double x2 = earthRadius * cos(lat2Rad) * cos(lon2Rad) + other.m_altitude * cos(lat2Rad) * cos(lon2Rad);
    double y2 = earthRadius * cos(lat2Rad) * sin(lon2Rad) + other.m_altitude * cos(lat2Rad) * sin(lon2Rad);
    double z2 = earthRadius * sin(lat2Rad) + other.m_altitude * sin(lat2Rad);
    
    // 计算欧几里得距离
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    
    return sqrt(dx * dx + dy * dy + dz * dz);
}

// 计算两点之间的精确球面距离（使用Vincenty公式）
double EarthPoint::vincentyDistanceTo(const EarthPoint& other) const {
    // WGS84椭球体参数
    const double a = 6378137.0;          // 长半轴（米）
    const double f = 1.0 / 298.257223563; // 扁率
    const double b = a * (1.0 - f);        // 短半轴（米）
    
    double lon1Rad = m_longitude * M_PI / 180.0;
    double lat1Rad = m_latitude * M_PI / 180.0;
    double lon2Rad = other.m_longitude * M_PI / 180.0;
    double lat2Rad = other.m_latitude * M_PI / 180.0;
    
    double L = lon2Rad - lon1Rad;
    double U1 = atan((1.0 - f) * tan(lat1Rad));
    double U2 = atan((1.0 - f) * tan(lat2Rad));
    
    double sinU1 = sin(U1);
    double cosU1 = cos(U1);
    double sinU2 = sin(U2);
    double cosU2 = cos(U2);
    
    double lambda = L;
    double lambdaP;
    double iterLimit = 100;
    double sinLambda, cosLambda, sinSigma, cosSigma, sigma, sinAlpha, cosSqAlpha, cos2SigmaM;
    
    do {
        sinLambda = sin(lambda);
        cosLambda = cos(lambda);
        sinSigma = sqrt((cosU2 * sinLambda) * (cosU2 * sinLambda) + 
                        (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda) * (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda));
        
        if (sinSigma == 0.0) {
            return 0.0; // 两点重合
        }
        
        cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
        sigma = atan2(sinSigma, cosSigma);
        sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
        cosSqAlpha = 1.0 - sinAlpha * sinAlpha;
        
        if (cosSqAlpha == 0.0) {
            cos2SigmaM = 0.0; // 赤道上的线
        } else {
            cos2SigmaM = cosSigma - 2.0 * sinU1 * sinU2 / cosSqAlpha;
        }
        
        double C = f / 16.0 * cosSqAlpha * (4.0 + f * (4.0 - 3.0 * cosSqAlpha));
        lambdaP = lambda;
        lambda = L + (1.0 - C) * f * sinAlpha * 
                 (sigma + C * sinSigma * (cos2SigmaM + C * cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM)));
        
    } while (fabs(lambda - lambdaP) > 1e-12 && --iterLimit > 0);
    
    if (iterLimit == 0) {
        return -1.0; // 迭代失败
    }
    
    double uSq = cosSqAlpha * (a * a - b * b) / (b * b);
    double A = 1.0 + uSq / 16384.0 * (4096.0 + uSq * (-768.0 + uSq * (320.0 - 175.0 * uSq)));
    double B = uSq / 1024.0 * (256.0 + uSq * (-128.0 + uSq * (74.0 - 47.0 * uSq)));
    double deltaSigma = B * sinSigma * (cos2SigmaM + B / 4.0 * (cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM) - 
                       B / 6.0 * cos2SigmaM * (-3.0 + 4.0 * sinSigma * sinSigma) * (-3.0 + 4.0 * cos2SigmaM * cos2SigmaM)));
    
    double s = b * A * (sigma - deltaSigma);
    
    // 考虑高度差
    double heightDiff = m_altitude - other.m_altitude;
    s = sqrt(s * s + heightDiff * heightDiff);
    
    return s;
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

// 计算两点之间的直线距离（欧几里得距离）
double straightLineDistance(const EarthPoint& lhs, const EarthPoint& rhs) {
    return lhs.straightLineDistanceTo(rhs);
}

// 计算两点之间的精确球面距离（使用Vincenty公式）
double vincentyDistance(const EarthPoint& lhs, const EarthPoint& rhs) {
    return lhs.vincentyDistanceTo(rhs);
}

} // namespace earth
} // namespace yalgo
