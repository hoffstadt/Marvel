cmake_minimum_required (VERSION 3.13)

add_executable (demo_vulkan)

set_property(TARGET demo_vulkan PROPERTY CXX_STANDARD 17)

target_sources(demo_vulkan

	PRIVATE

		"main.cpp"
)

target_include_directories(demo_vulkan

	PRIVATE
	    "$ENV{VK_SDK_PATH}/Include"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
)

target_link_directories(demo_vulkan

	PRIVATE

		"$ENV{VK_SDK_PATH}/Lib"

)

target_link_libraries(demo_vulkan

	PRIVATE

		vulkan-1
		glfw
)
