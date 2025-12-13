#include "math_utils.h"

namespace yalgo {
namespace math {

// 角度转弧度
double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// 弧度转角度
double radiansToDegrees(double radians) {
    return radians * 180.0 / M_PI;
}

// 瓦特转千瓦
double wattsToKilowatts(double watts) {
    return watts / 1000.0;
}

// 千瓦转瓦特
double kilowattsToWatts(double kilowatts) {
    return kilowatts * 1000.0;
}

// 瓦特转马力 (公制马力)
double wattsToHorsepower(double watts) {
    // 1公制马力 = 735.49875瓦特
    return watts / 735.49875;
}

// 马力转瓦特 (公制马力)
double horsepowerToWatts(double horsepower) {
    // 1公制马力 = 735.49875瓦特
    return horsepower * 735.49875;
}

// 瓦特转dBW (dB相对于1瓦特)
double wattsToDbW(double watts) {
    return 10.0 * std::log10(watts);
}

// dBW转瓦特
double dbWToWatts(double dbW) {
    return std::pow(10.0, dbW / 10.0);
}

// 瓦特转dBm (dB相对于1毫瓦特)
double wattsToDbm(double watts) {
    return 10.0 * std::log10(watts / 0.001);
}

// dBm转瓦特
double dbmToWatts(double dbm) {
    return 0.001 * std::pow(10.0, dbm / 10.0);
}

// dBW转dBm
double dbWToDbm(double dbW) {
    return dbW + 30.0; // 1瓦特 = 1000毫瓦特，所以+30dB
}

// dBm转dBW
double dbmToDbW(double dbm) {
    return dbm - 30.0; // 1毫瓦特 = 0.001瓦特，所以-30dB
}

// 通用线性值转分贝值
double linearToDb(double linear, double reference) {
    return 10.0 * std::log10(linear / reference);
}

// 通用分贝值转线性值
double dbToLinear(double db, double reference) {
    return reference * std::pow(10.0, db / 10.0);
}

} // namespace math
} // namespace yalgo
