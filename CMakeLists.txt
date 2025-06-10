cmake_minimum_required(VERSION 3.10)
project(cmake_project1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(VULKAN_SDK "C:/VulkanSDK/1.3.290.0")


find_package(Vulkan REQUIRED)


include_directories(${VULKAN_SDK}/Include)
link_directories(${VULKAN_SDK}/Lib)

include_directories("C:/Users/jensm/Desktop/local/visual studio/glfw-3.4.bin.WIN64/include")
link_directories("C:/Users/jensm/Desktop/local/visual studio/glfw-3.4.bin.WIN64/lib-vc2022")


include_directories("C:/Users/jensm/Desktop/local/glm-1.0.1")

# Source files
add_executable(cmake_project1
                src/main.cpp
                src/VulkanContext.h
                src/VulkanContext.cpp
                src/VulkanDevice.h
                src/VulkanDevice.cpp
                src/VulkanDebug.h
                src/VulkanDebug.cpp
                src/VulkanSwapchain.h
                src/VulkanSwapchain.cpp
                src/VulkanPipeline.cpp
                src/VulkanPipeline.h
                src/VulkanRenderer.cpp
                src/VulkanRenderer.h
                src/VulkanResource.cpp
                src/VulkanResource.h
                src/VulkanUtils.cpp
                src/VulkanUtils.h
                )

target_link_libraries(cmake_project1 glfw3 vulkan-1)
