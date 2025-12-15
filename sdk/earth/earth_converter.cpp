#define _USE_MATH_DEFINES
#include "earth_converter.h"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace yalgo {
namespace earth {

// UTM坐标的字符串表示
std::string EarthConverter::UTMCoordinate::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "UTM(easting: " << easting << ", northing: " << northing << ", zone: " << zone << ", hemisphere: " << hemisphere << ")";
    return oss.str();
}

// ECEF坐标的字符串表示
std::string EarthConverter::ECEFCoordinate::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "ECEF(x: " << x << ", y: " << y << ", z: " << z << ")";
    return oss.str();
}

// 墨卡托坐标的字符串表示
std::string EarthConverter::MercatorCoordinate::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "Mercator(x: " << x << ", y: " << y << ")";
    return oss.str();
}

// 获取椭球体参数
EarthConverter::EllipsoidParams EarthConverter::getEllipsoidParams(Ellipsoid ellipsoid) const {
    EllipsoidParams params;
    
    switch (ellipsoid) {
        case Ellipsoid::WGS84:
            params.semiMajorAxis = 6378137.0;
            params.flattening = 1.0 / 298.257223563;
            break;
        case Ellipsoid::GRS80:
            params.semiMajorAxis = 6378137.0;
            params.flattening = 1.0 / 298.257222101;
            break;
        case Ellipsoid::CLARKE1866:
            params.semiMajorAxis = 6378206.4;
            params.flattening = 1.0 / 294.978698214;
            break;
        case Ellipsoid::AIRY:
            params.semiMajorAxis = 6377563.396;
            params.flattening = 1.0 / 299.3249646;
            break;
        case Ellipsoid::BESSEL1841:
            params.semiMajorAxis = 6377397.155;
            params.flattening = 1.0 / 299.1528128;
            break;
        default:
            params.semiMajorAxis = 6378137.0;
            params.flattening = 1.0 / 298.257223563;
            break;
    }
    
    params.semiMinorAxis = params.semiMajorAxis * (1.0 - params.flattening);
    params.eccentricitySquared = 2.0 * params.flattening - params.flattening * params.flattening;
    params.secondEccentricitySquared = params.eccentricitySquared / (1.0 - params.eccentricitySquared);
    
    return params;
}

// 默认构造函数
EarthConverter::EarthConverter() : m_ellipsoid(Ellipsoid::WGS84) {
    m_params = getEllipsoidParams(m_ellipsoid);
}

// 带椭球体参数的构造函数
EarthConverter::EarthConverter(Ellipsoid ellipsoid) : m_ellipsoid(ellipsoid) {
    m_params = getEllipsoidParams(m_ellipsoid);
}

// 设置椭球体模型
void EarthConverter::setEllipsoid(Ellipsoid ellipsoid) {
    m_ellipsoid = ellipsoid;
    m_params = getEllipsoidParams(m_ellipsoid);
}

// 获取当前椭球体模型
EarthConverter::Ellipsoid EarthConverter::getEllipsoid() const {
    return m_ellipsoid;
}

// WGS84经纬度坐标转换为ECEF笛卡尔坐标
EarthConverter::ECEFCoordinate EarthConverter::wgs84ToECEF(const EarthPoint& point) const {
    double lonRad = point.longitude() * M_PI / 180.0;
    double latRad = point.latitude() * M_PI / 180.0;
    double alt = point.altitude();
    
    double cosLon = std::cos(lonRad);
    double sinLon = std::sin(lonRad);
    double cosLat = std::cos(latRad);
    double sinLat = std::sin(latRad);
    
    // 计算N(卯酉圈曲率半径)
    double N = m_params.semiMajorAxis / std::sqrt(1.0 - m_params.eccentricitySquared * sinLat * sinLat);
    
    // 计算ECEF坐标
    ECEFCoordinate ecef;
    ecef.x = (N + alt) * cosLat * cosLon;
    ecef.y = (N + alt) * cosLat * sinLon;
    ecef.z = (N * (1.0 - m_params.eccentricitySquared) + alt) * sinLat;
    
    return ecef;
}

// ECEF笛卡尔坐标转换为WGS84经纬度坐标
EarthPoint EarthConverter::ecefToWGS84(const ECEFCoordinate& ecef) const {
    double x = ecef.x;
    double y = ecef.y;
    double z = ecef.z;
    
    double p = std::sqrt(x * x + y * y);
    double theta = std::atan2(z * m_params.semiMajorAxis, p * m_params.semiMinorAxis);
    
    double sinTheta = std::sin(theta);
    double cosTheta = std::cos(theta);
    
    // 计算纬度
    double latRad = std::atan2(z + m_params.secondEccentricitySquared * m_params.semiMinorAxis * sinTheta * sinTheta * sinTheta,
                             p - m_params.eccentricitySquared * m_params.semiMajorAxis * cosTheta * cosTheta * cosTheta);
    
    // 计算经度
    double lonRad = std::atan2(y, x);
    
    // 计算N(卯酉圈曲率半径)
    double sinLat = std::sin(latRad);
    double N = m_params.semiMajorAxis / std::sqrt(1.0 - m_params.eccentricitySquared * sinLat * sinLat);
    
    // 计算高度
    double alt = p / std::cos(latRad) - N;
    
    // 转换为度
    double lonDeg = lonRad * 180.0 / M_PI;
    double latDeg = latRad * 180.0 / M_PI;
    
    return EarthPoint(lonDeg, latDeg, alt);
}

// WGS84经纬度坐标转换为UTM坐标
EarthConverter::UTMCoordinate EarthConverter::wgs84ToUTM(const EarthPoint& point) const {
    double lonDeg = point.longitude();
    double latDeg = point.latitude();
    
    // 计算UTM带号
    int zone = static_cast<int>((lonDeg + 180.0) / 6.0) + 1;
    
    // 特殊处理挪威和斯瓦尔巴群岛
    if (latDeg >= 56.0 && latDeg < 64.0 && lonDeg >= 3.0 && lonDeg < 12.0) {
        zone = 32;
    }
    if (latDeg >= 72.0 && latDeg < 84.0) {
        if (lonDeg >= 0.0 && lonDeg < 9.0) zone = 31;
        else if (lonDeg >= 9.0 && lonDeg < 21.0) zone = 33;
        else if (lonDeg >= 21.0 && lonDeg < 33.0) zone = 35;
        else if (lonDeg >= 33.0 && lonDeg < 42.0) zone = 37;
    }
    
    // 计算中央经线
    double lon0Deg = (zone - 1) * 6.0 - 180.0 + 3.0;
    double lon0Rad = lon0Deg * M_PI / 180.0;
    double lonRad = lonDeg * M_PI / 180.0;
    double latRad = latDeg * M_PI / 180.0;
    
    double cosLat = std::cos(latRad);
    double sinLat = std::sin(latRad);
    double tanLat = std::tan(latRad);
    
    // 计算UTM投影参数
    double N = m_params.semiMajorAxis / std::sqrt(1.0 - m_params.eccentricitySquared * sinLat * sinLat);
    double T = tanLat * tanLat;
    double C = m_params.eccentricitySquared * cosLat * cosLat / (1.0 - m_params.eccentricitySquared);
    double A = cosLat * (lonRad - lon0Rad);
    double M = m_params.semiMajorAxis * ((1.0 - m_params.eccentricitySquared / 4.0 - 3.0 * m_params.eccentricitySquared * m_params.eccentricitySquared / 64.0 - 5.0 * m_params.eccentricitySquared * m_params.eccentricitySquared * m_params.eccentricitySquared / 256.0) * latRad
                   - (3.0 * m_params.eccentricitySquared / 8.0 + 3.0 * m_params.eccentricitySquared * m_params.eccentricitySquared / 32.0 + 45.0 * m_params.eccentricitySquared * m_params.eccentricitySquared * m_params.eccentricitySquared / 1024.0) * std::sin(2.0 * latRad)
                   + (15.0 * m_params.eccentricitySquared * m_params.eccentricitySquared / 256.0 + 45.0 * m_params.eccentricitySquared * m_params.eccentricitySquared * m_params.eccentricitySquared / 1024.0) * std::sin(4.0 * latRad)
                   - (35.0 * m_params.eccentricitySquared * m_params.eccentricitySquared * m_params.eccentricitySquared / 3072.0) * std::sin(6.0 * latRad));
    
    // 计算东向和北向坐标
    double easting = 500000.0 + N * (A + (1.0 - T + C) * A * A * A / 6.0 + (5.0 - 18.0 * T + T * T + 72.0 * C - 58.0 * m_params.secondEccentricitySquared) * A * A * A * A * A / 120.0);
    double northing = M + N * tanLat * (A * A / 2.0 + (5.0 - T + 9.0 * C + 4.0 * C * C) * A * A * A * A / 24.0 + (61.0 - 58.0 * T + T * T + 600.0 * C - 330.0 * m_params.secondEccentricitySquared) * A * A * A * A * A * A / 720.0);
    
    // 南半球调整
    char hemisphere = 'N';
    if (latDeg < 0.0) {
        hemisphere = 'S';
        northing += 10000000.0;
    }
    
    UTMCoordinate utm;
    utm.easting = easting;
    utm.northing = northing;
    utm.zone = zone;
    utm.hemisphere = hemisphere;
    
    return utm;
}

// UTM坐标转换为WGS84经纬度坐标
EarthPoint EarthConverter::utmToWGS84(const UTMCoordinate& utm) const {
    double easting = utm.easting;
    double northing = utm.northing;
    int zone = utm.zone;
    char hemisphere = utm.hemisphere;
    
    // 南半球调整
    if (hemisphere == 'S' || hemisphere == 's') {
        northing -= 10000000.0;
    }
    
    // 计算中央经线
    double lon0Deg = (zone - 1) * 6.0 - 180.0 + 3.0;
    double lon0Rad = lon0Deg * M_PI / 180.0;
    
    // 计算UTM投影参数
    double k0 = 0.9996; // UTM缩放因子
    double easting0 = 500000.0;
    
    double eastingPrime = easting - easting0;
    double M = northing / k0;
    
    // 迭代计算纬度
    double mu = M / (m_params.semiMajorAxis * (1.0 - m_params.eccentricitySquared / 4.0 - 3.0 * m_params.eccentricitySquared * m_params.eccentricitySquared / 64.0 - 5.0 * m_params.eccentricitySquared * m_params.eccentricitySquared * m_params.eccentricitySquared / 256.0));
    double e1 = (1.0 - std::sqrt(1.0 - m_params.eccentricitySquared)) / (1.0 + std::sqrt(1.0 - m_params.eccentricitySquared));
    
    double phi1Rad = mu + (3.0 * e1 / 2.0 - 27.0 * e1 * e1 * e1 / 32.0) * std::sin(2.0 * mu) + (21.0 * e1 * e1 / 16.0 - 55.0 * e1 * e1 * e1 * e1 / 32.0) * std::sin(4.0 * mu) + (151.0 * e1 * e1 * e1 / 96.0) * std::sin(6.0 * mu) + (1097.0 * e1 * e1 * e1 * e1 / 512.0) * std::sin(8.0 * mu);
    
    double sinPhi1 = std::sin(phi1Rad);
    double cosPhi1 = std::cos(phi1Rad);
    double tanPhi1 = std::tan(phi1Rad);
    
    double N1 = m_params.semiMajorAxis / std::sqrt(1.0 - m_params.eccentricitySquared * sinPhi1 * sinPhi1);
    double T1 = tanPhi1 * tanPhi1;
    double C1 = m_params.eccentricitySquared * cosPhi1 * cosPhi1 / (1.0 - m_params.eccentricitySquared);
    double R1 = m_params.semiMajorAxis * (1.0 - m_params.eccentricitySquared) / std::pow(1.0 - m_params.eccentricitySquared * sinPhi1 * sinPhi1, 1.5);
    double D = eastingPrime / (N1 * k0);
    
    // 计算纬度
    double latRad = phi1Rad - (N1 * tanPhi1 / R1) * (D * D / 2.0 - (5.0 + 3.0 * T1 + 10.0 * C1 - 4.0 * C1 * C1 - 9.0 * m_params.secondEccentricitySquared) * D * D * D * D / 24.0 + (61.0 + 90.0 * T1 + 298.0 * C1 + 45.0 * T1 * T1 - 252.0 * m_params.secondEccentricitySquared - 3.0 * C1 * C1) * D * D * D * D * D * D / 720.0);
    
    // 计算经度
    double lonRad = lon0Rad + (D - (1.0 + 2.0 * T1 + C1) * D * D * D / 6.0 + (5.0 - 2.0 * C1 + 28.0 * T1 - 3.0 * C1 * C1 + 8.0 * m_params.secondEccentricitySquared + 24.0 * T1 * T1) * D * D * D * D * D / 120.0) / cosPhi1;
    
    // 转换为度
    double lonDeg = lonRad * 180.0 / M_PI;
    double latDeg = latRad * 180.0 / M_PI;
    
    return EarthPoint(lonDeg, latDeg, 0.0);
}

// 获取椭球体的长半轴
double EarthConverter::getSemiMajorAxis() const {
    return m_params.semiMajorAxis;
}

// 获取椭球体的扁率
double EarthConverter::getFlattening() const {
    return m_params.flattening;
}

// 获取椭球体的短半轴
double EarthConverter::getSemiMinorAxis() const {
    return m_params.semiMinorAxis;
}

// 获取椭球体的第一偏心率平方
double EarthConverter::getEccentricitySquared() const {
    return m_params.eccentricitySquared;
}

// 获取椭球体的第二偏心率平方
double EarthConverter::getSecondEccentricitySquared() const {
    return m_params.secondEccentricitySquared;
}

// 判断地球上两点是否通视
bool EarthConverter::isVisible(const EarthPoint& point1, const EarthPoint& point2) const {
    // 将WGS84坐标转换为ECEF坐标
    ECEFCoordinate ecef1 = wgs84ToECEF(point1);
    ECEFCoordinate ecef2 = wgs84ToECEF(point2);
    
    // 计算向量AB（从点A到点B）
    double ABx = ecef2.x - ecef1.x;
    double ABy = ecef2.y - ecef1.y;
    double ABz = ecef2.z - ecef1.z;
    
    // 计算向量OA（从原点到点A）
    double OAx = ecef1.x;
    double OAy = ecef1.y;
    double OAz = ecef1.z;
    
    // 计算点积 OA · AB
    double dotProduct = OAx * ABx + OAy * ABy + OAz * ABz;
    
    // 如果点积为正，说明最小距离在点A的另一侧，两点通视
    if (dotProduct >= 0.0) {
        return true;
    }
    
    // 计算AB向量的长度平方
    double ABLengthSquared = ABx * ABx + ABy * ABy + ABz * ABz;
    
    // 如果AB向量长度为0，说明两点重合，通视
    if (ABLengthSquared == 0.0) {
        return true;
    }
    
    // 计算参数t，用于确定投影点在AB线段上的位置
    double t = -dotProduct / ABLengthSquared;
    
    // 计算投影点P的坐标
    double Px, Py, Pz;
    if (t <= 0.0) {
        // 投影点在点A或其后方
        Px = ecef1.x;
        Py = ecef1.y;
        Pz = ecef1.z;
    } else if (t >= 1.0) {
        // 投影点在点B或其前方
        Px = ecef2.x;
        Py = ecef2.y;
        Pz = ecef2.z;
    } else {
        // 投影点在线段AB上
        Px = ecef1.x + t * ABx;
        Py = ecef1.y + t * ABy;
        Pz = ecef1.z + t * ABz;
    }
    
    // 计算投影点P到原点的距离
    double PLength = std::sqrt(Px * Px + Py * Py + Pz * Pz);
    
    // 如果投影点到原点的距离大于地球半径，则两点通视
    // 使用椭球体的长半轴作为地球半径近似值
    return PLength > m_params.semiMajorAxis;
}

// WGS84经纬度坐标转换为墨卡托坐标
EarthConverter::MercatorCoordinate EarthConverter::wgs84ToMercator(const EarthPoint& point) const {
    double lonDeg = point.longitude();
    double latDeg = point.latitude();
    
    // 墨卡托投影的纬度范围限制
    if (latDeg < -85.05 || latDeg > 85.05) {
        return MercatorCoordinate(0.0, 0.0);
    }
    
    double lonRad = lonDeg * M_PI / 180.0;
    double latRad = latDeg * M_PI / 180.0;
    
    double x = m_params.semiMajorAxis * lonRad;
    double y = m_params.semiMajorAxis * log(tan(M_PI / 4.0 + latRad / 2.0) * pow((1 - sqrt(m_params.eccentricitySquared) * sin(latRad)) / (1 + sqrt(m_params.eccentricitySquared) * sin(latRad)), sqrt(m_params.eccentricitySquared) / 2.0));
    
    return MercatorCoordinate(x, y);
}

// 墨卡托坐标转换为WGS84经纬度坐标
EarthPoint EarthConverter::mercatorToWGS84(const MercatorCoordinate& mercator) const {
    double x = mercator.x;
    double y = mercator.y;
    
    // 墨卡托坐标范围限制
    if (fabs(x) > 20037508.34 || fabs(y) > 20037508.34) {
        return EarthPoint(0.0, 0.0, 0.0);
    }
    
    double lonRad = x / m_params.semiMajorAxis;
    double latRad = 2 * (atan(exp(y / m_params.semiMajorAxis)) - M_PI / 4.0);
    
    // 迭代计算纬度（3次迭代足以达到精度要求）
    for (int i = 0; i < 3; i++) {
        double eSinLat = sqrt(m_params.eccentricitySquared) * sin(latRad);
        latRad = 2 * (atan(exp(y / m_params.semiMajorAxis) * pow((1 + eSinLat) / (1 - eSinLat), sqrt(m_params.eccentricitySquared) / 2.0)) - M_PI / 4.0);
    }
    
    double lonDeg = lonRad * 180.0 / M_PI;
    double latDeg = latRad * 180.0 / M_PI;
    
    return EarthPoint(lonDeg, latDeg, 0.0);
}

} // namespace earth
} // namespace yalgo
