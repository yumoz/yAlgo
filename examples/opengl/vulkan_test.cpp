#include "vulkan_test.h"
#include <iostream>
#include <stdexcept>
#include <vector>

VulkanTest::VulkanTest() : instance(VK_NULL_HANDLE) {
    earthPoint = new EarthPoint(116.3974, 39.9093, 50.0);
}

VulkanTest::~VulkanTest() {
    delete earthPoint;
    cleanup();
}

void VulkanTest::init() {
    std::cout << "Initializing Vulkan..." << std::endl;
    
    if (!checkVulkanSupport()) {
        throw std::runtime_error("Vulkan is not supported on this system!");
    }
    
    createInstance();
    std::cout << "Vulkan initialization successful!" << std::endl;
}

void VulkanTest::run() {
    std::cout << "Running Vulkan test..." << std::endl;
    
    // 打印地球坐标点信息
    std::cout << "Earth point: " << earthPoint->toString() << std::endl;
    
    // 打印Vulkan设备信息
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (deviceCount == 0) {
        std::cout << "No Vulkan physical devices found!" << std::endl;
    } else {
        std::cout << "Found " << deviceCount << " Vulkan physical device(s)!" << std::endl;
        
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        
        for (uint32_t i = 0; i < deviceCount; i++) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(devices[i], &properties);
            std::cout << "  Device " << i + 1 << ": " << properties.deviceName << std::endl;
            std::cout << "    API version: " << ((properties.apiVersion >> 22) & 0x3FF) << "." << ((properties.apiVersion >> 12) & 0x3FF) << "." << (properties.apiVersion & 0xFFF) << std::endl;
            std::cout << "    Device type: ";
            switch (properties.deviceType) {
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    std::cout << "Integrated GPU" << std::endl;
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    std::cout << "Discrete GPU" << std::endl;
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    std::cout << "Virtual GPU" << std::endl;
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    std::cout << "CPU" << std::endl;
                    break;
                default:
                    std::cout << "Unknown" << std::endl;
                    break;
            }
        }
    }
    
    std::cout << "Vulkan test completed successfully!" << std::endl;
}

void VulkanTest::cleanup() {
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
}

void VulkanTest::createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Earth SDK Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
}

bool VulkanTest::checkVulkanSupport() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    
    std::cout << "Available Vulkan extensions: " << extensionCount << std::endl;
    for (const auto& extension : extensions) {
        std::cout << "  - " << extension.extensionName << std::endl;
    }
    
    return true;
}
