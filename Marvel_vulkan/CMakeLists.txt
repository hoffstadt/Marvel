cmake_minimum_required (VERSION 3.13)

add_executable (marvel_vulkan)

set_property(TARGET marvel_vulkan PROPERTY CXX_STANDARD 17)

target_sources(marvel_vulkan

	PRIVATE

		"main.cpp"

		"core/mvWindow.cpp"

		"renderer/mvGraphics.cpp"
)

target_include_directories(marvel_vulkan

	PRIVATE
		"core/"
		"renderer/"
	    "$ENV{VK_SDK_PATH}/Include"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
)

target_link_directories(marvel_vulkan

	PRIVATE
		
		"$ENV{VK_SDK_PATH}/Lib"

)

target_link_libraries(marvel_vulkan

	PRIVATE

		vulkan-1
		glfw
)
