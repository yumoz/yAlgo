#ifndef VULKAN_TEST_H
#define VULKAN_TEST_H

#include <vulkan/vulkan.h>
#include "../../sdk/earth/earth_point.h"

using namespace yalgo::earth;

class VulkanTest {
private:
    // Vulkan实例
    VkInstance instance;

    // 地球坐标点
    EarthPoint* earthPoint;

public:
    VulkanTest();
    ~VulkanTest();

    // 初始化Vulkan
    void init();

    // 运行测试
    void run();

    // 清理资源
    void cleanup();

private:
    // 创建Vulkan实例
    void createInstance();

    // 检查Vulkan支持
    bool checkVulkanSupport();
};

#endif // VULKAN_TEST_H
