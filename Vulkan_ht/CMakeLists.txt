cmake_minimum_required (VERSION 3.13)

add_executable (vulkan_ht)

set_property(TARGET vulkan_ht PROPERTY CXX_STANDARD 17)

target_sources(vulkan_ht

	PRIVATE

		"main.cpp"
)

target_include_directories(vulkan_ht

	PRIVATE
	    "$ENV{VK_SDK_PATH}/Include"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
)

target_link_directories(vulkan_ht

	PRIVATE

		"$ENV{VK_SDK_PATH}/Lib"

)

target_link_libraries(vulkan_ht

	PRIVATE

		vulkan-1
		glfw
)
