#pragma once

#include "earth_exports.h"
#include "earth_point.h"
#include "earth_converter.h"
#include <vector>
#include <string>

namespace yalgo {
namespace earth {

/**
 * @brief 地球几何计算类
 * 
 * 实现地球上的几何计算功能，包括：
 * - 点在多边形内的判断
 * - 投影精度统计
 * - GEO卫星覆盖测试
 */
class EARTH_API EarthGeometry {
public:
    /**
     * @brief 投影类型枚举
     */
    enum class ProjectionType {
        UTM,      ///< UTM投影
        MERCATOR  ///< 墨卡托投影
    };

    /**
     * @brief 判断点是否在多边形内
     * 
     * @param point 要判断的点
     * @param polygon 多边形顶点集合
     * @param projectionType 投影类型，默认UTM
     * @return bool 如果点在多边形内返回true，否则返回false
     */
    bool isPointInPolygon(const EarthPoint& point, const std::vector<EarthPoint>& polygon, ProjectionType projectionType = ProjectionType::UTM) const;

    /**
     * @brief 计算投影精度统计
     * 
     * @param testPoints 测试点集合
     * @param projectionType 投影类型，默认UTM
     */
    void calcProjectionAccuracy(const std::vector<EarthPoint>& testPoints, ProjectionType projectionType = ProjectionType::UTM) const;

    /**
     * @brief GEO卫星中国区域覆盖测试
     * 
     * 模拟GEO卫星（105°E赤道上空）的中国主覆盖区，测试典型城市是否在覆盖内
     */
    void testGEOCoverageInChina() const;
};

} // namespace earth
} // namespace yalgo