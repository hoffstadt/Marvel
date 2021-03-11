#include "mvDevice.h"
#include "mvGraphics.h"

namespace Marvel {


    const std::vector<const char*> mvDevice::s_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };


    bool mvDevice::IsDeviceSuitable(mvGraphics& graphics, VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = graphics.getSurface().findQueueFamilies(device);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) 
        {
            SwapChainSupportDetails swapChainSupport = graphics.getSurface().querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    void mvDevice::init(mvGraphics& graphics)
    {
        pickPhysicalDevice(graphics);
        createLogicalDevice(graphics);
    }

    void mvDevice::cleanup(mvGraphics& graphics)
    {
        vkDestroyDevice(m_device, nullptr);
    }

    void mvDevice::pickPhysicalDevice(mvGraphics& graphics)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(graphics.getInstance(), &deviceCount, nullptr);

        if (deviceCount == 0)
            throw std::runtime_error("failed to find GPUs with Vulkan support!");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(graphics.getInstance(), &deviceCount, devices.data());

        for (const auto& device : devices)
        {
            if (IsDeviceSuitable(graphics, device))
            {
                m_physicalDevice = device;
                break;
            }
        }

        if (m_physicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("failed to find a suitable GPU!");
    }

    void mvDevice::createLogicalDevice(mvGraphics& graphics)
    {
        QueueFamilyIndices indices = graphics.getSurface().findQueueFamilies(m_physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        {
            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

            createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
            createInfo.pQueueCreateInfos = queueCreateInfos.data();

            createInfo.pEnabledFeatures = &deviceFeatures;

            createInfo.enabledExtensionCount = static_cast<uint32_t>(s_deviceExtensions.size());
            createInfo.ppEnabledExtensionNames = s_deviceExtensions.data();

            if (mvGraphics::UseValidationLayers())
            {
                createInfo.enabledLayerCount = static_cast<uint32_t>(mvGraphics::GetValidationLayers().size());
                createInfo.ppEnabledLayerNames = mvGraphics::GetValidationLayers().data();
            }
            else
                createInfo.enabledLayerCount = 0;

            if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
                throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
    }

    uint32_t mvDevice::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    bool mvDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(s_deviceExtensions.begin(), s_deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

}