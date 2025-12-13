#ifndef YALGO_EXAMPLES_UTILS_UTILS_TEST_H
#define YALGO_EXAMPLES_UTILS_UTILS_TEST_H

#include "../../sdk/utils/exec_path_utils.h"
#include <iostream>

namespace yalgo {
namespace examples {

/**
 * 工具模块测试类
 * 封装所有工具功能的测试方法
 */
class UtilsTest {
public:
    /**
     * 演示获取可执行文件完整路径
     */
    static void demoExecutablePath();
    
    /**
     * 演示获取可执行文件目录
     */
    static void demoExecutableDir();
    
    /**
     * 演示在可执行文件目录下拼接文件路径
     */
    static void demoFileInExeDir();
    
    /**
     * 演示检查可执行文件目录下的文件是否存在
     */
    static void demoCheckFileInExeDir();
    
    /**
     * 运行所有测试
     */
    static void runAllTests();
};

} // namespace examples
} // namespace yalgo

#endif // YALGO_EXAMPLES_UTILS_UTILS_TEST_H