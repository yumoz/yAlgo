#include "vulkan_test.h"
#include <iostream>

int main() {
    try {
        std::cout << "Vulkan Earth SDK Test" << std::endl;
        std::cout << "====================" << std::endl;
        
        // 创建Vulkan测试对象
        VulkanTest test;
        
        // 初始化Vulkan
        std::cout << "Initializing Vulkan..." << std::endl;
        test.init();
        
        // 运行主循环
        std::cout << "Running Vulkan test..." << std::endl;
        std::cout << "Press ESC to exit." << std::endl;
        test.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
