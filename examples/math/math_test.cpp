#include "math_test.h"
#include <iomanip>

namespace yalgo {
namespace examples {

using namespace yalgo::math;

// 演示角度到弧度转换
void MathTest::demoDegreesToRadians() {
    std::cout << "\n=== 角度到弧度转换 ===\n";
    
    double degrees[] = {0, 30, 45, 60, 90, 180, 270, 360};
    
    for (double deg : degrees) {
        double rad = degreesToRadians(deg);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << deg << "度 = " << rad << "弧度\n";
    }
}

// 演示弧度到角度转换
void MathTest::demoRadiansToDegrees() {
    std::cout << "\n=== 弧度到角度转换 ===\n";
    
    double radians[] = {0, M_PI/6, M_PI/4, M_PI/3, M_PI/2, M_PI, 3*M_PI/2, 2*M_PI};
    
    for (double rad : radians) {
        double deg = radiansToDegrees(rad);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << rad << "弧度 = " << deg << "度\n";
    }
}

// 演示功率单位转换（瓦特到千瓦）
void MathTest::demoWattsToKilowatts() {
    std::cout << "\n=== 瓦特到千瓦转换 ===\n";
    
    double watts[] = {500, 1000, 1500, 2000, 3000, 5000, 10000};
    
    for (double w : watts) {
        double kw = wattsToKilowatts(w);
        std::cout << std::fixed << std::setprecision(3);
        std::cout << w << "瓦特 = " << kw << "千瓦\n";
    }
}

// 演示功率单位转换（千瓦到瓦特）
void MathTest::demoKilowattsToWatts() {
    std::cout << "\n=== 千瓦到瓦特转换 ===\n";
    
    double kilowatts[] = {0.5, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0};
    
    for (double kw : kilowatts) {
        double w = kilowattsToWatts(kw);
        std::cout << std::fixed << std::setprecision(0);
        std::cout << kw << "千瓦 = " << w << "瓦特\n";
    }
}

// 演示功率单位转换（瓦特到马力）
void MathTest::demoWattsToHorsepower() {
    std::cout << "\n=== 瓦特到马力转换 ===\n";
    
    double watts[] = {735.5, 1000, 5000, 10000, 50000, 100000};
    
    for (double w : watts) {
        double hp = wattsToHorsepower(w);
        std::cout << std::fixed << std::setprecision(3);
        std::cout << w << "瓦特 = " << hp << "马力\n";
    }
}

// 演示功率单位转换（马力到瓦特）
void MathTest::demoHorsepowerToWatts() {
    std::cout << "\n=== 马力到瓦特转换 ===\n";
    
    double horsepower[] = {1.0, 1.36, 5.0, 10.0, 50.0, 100.0};
    
    for (double hp : horsepower) {
        double w = horsepowerToWatts(hp);
        std::cout << std::fixed << std::setprecision(0);
        std::cout << hp << "马力 = " << w << "瓦特\n";
    }
}

// 演示瓦特到dBW转换
void MathTest::demoWattsToDbW() {
    std::cout << "\n=== 瓦特到dBW转换 ===\n";
    
    double watts[] = {0.1, 1.0, 10.0, 100.0, 1000.0, 10000.0};
    
    for (double w : watts) {
        double dbw = wattsToDbW(w);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << w << "瓦特 = " << dbw << "dBW\n";
    }
}

// 演示dBW到瓦特转换
void MathTest::demoDbWToWatts() {
    std::cout << "\n=== dBW到瓦特转换 ===\n";
    
    double dbw[] = {-10.0, 0.0, 10.0, 20.0, 30.0, 40.0};
    
    for (double db : dbw) {
        double w = dbWToWatts(db);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << db << "dBW = " << w << "瓦特\n";
    }
}

// 演示瓦特到dBm转换
void MathTest::demoWattsToDbm() {
    std::cout << "\n=== 瓦特到dBm转换 ===\n";
    
    double watts[] = {0.000001, 0.001, 0.01, 0.1, 1.0, 10.0};
    
    for (double w : watts) {
        double dbm = wattsToDbm(w);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << w << "瓦特 = " << dbm << "dBm\n";
    }
}

// 演示dBm到瓦特转换
void MathTest::demoDbmToWatts() {
    std::cout << "\n=== dBm到瓦特转换 ===\n";
    
    double dbm[] = {0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0};
    
    for (double db : dbm) {
        double w = dbmToWatts(db);
        std::cout << std::fixed << std::setprecision(6);
        std::cout << db << "dBm = " << w << "瓦特\n";
    }
}

// 演示dBW和dBm之间的转换
void MathTest::demoDbWAndDbmConversion() {
    std::cout << "\n=== dBW和dBm之间的转换 ===\n";
    
    double dbw[] = {-10.0, 0.0, 10.0, 20.0};
    
    for (double db : dbw) {
        double dbm = dbWToDbm(db);
        double convertedBackDbw = dbmToDbW(dbm);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << db << "dBW = " << dbm << "dBm = " << convertedBackDbw << "dBW (验证)\n";
    }
}

// 演示通用分贝转换
void MathTest::demoGeneralDbConversion() {
    std::cout << "\n=== 通用分贝转换 ===\n";
    
    // 测试线性值到分贝值的转换
    double linearValues[] = {0.01, 0.1, 1.0, 10.0, 100.0};
    std::cout << "线性值到分贝值（参考值=1.0）：\n";
    for (double linear : linearValues) {
        double db = linearToDb(linear);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << linear << " = " << db << "dB\n";
    }
    
    // 测试分贝值到线性值的转换
    double dbValues[] = {-20.0, -10.0, 0.0, 10.0, 20.0};
    std::cout << "\n分贝值到线性值（参考值=1.0）：\n";
    for (double db : dbValues) {
        double linear = dbToLinear(db);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << db << "dB = " << linear << "\n";
    }
    
    // 测试自定义参考值的转换
    std::cout << "\n使用自定义参考值（参考值=2.0）：\n";
    double testValue = 8.0;
    double dbWithCustomRef = linearToDb(testValue, 2.0);
    double linearWithCustomRef = dbToLinear(dbWithCustomRef, 2.0);
    std::cout << std::fixed << std::setprecision(4);
    std::cout << testValue << "（参考值=2.0） = " << dbWithCustomRef << "dB\n";
    std::cout << dbWithCustomRef << "dB（参考值=2.0） = " << linearWithCustomRef << "（验证）\n";
}

// 运行所有测试
void MathTest::runAllTests() {
    std::cout << "========================================\n";
    std::cout << "yAlgo 数学工具库测试\n";
    std::cout << "========================================\n";
    
    // 运行所有测试方法
    demoDegreesToRadians();
    demoRadiansToDegrees();
    demoWattsToKilowatts();
    demoKilowattsToWatts();
    demoWattsToHorsepower();
    demoHorsepowerToWatts();
    demoWattsToDbW();
    demoDbWToWatts();
    demoWattsToDbm();
    demoDbmToWatts();
    demoDbWAndDbmConversion();
    demoGeneralDbConversion();
    
    std::cout << "\n========================================\n";
    std::cout << "所有数学工具库测试完成\n";
    std::cout << "========================================\n";
}

} // namespace examples
} // namespace yalgo
