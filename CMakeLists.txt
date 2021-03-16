cmake_minimum_required (VERSION 3.13)

project ("Marvel")

set(MV_VULKAN ${MV_VULKAN})
set(MV_D3D11 ${MV_D3D11})
set(MV_D3D12 ${MV_D3D12})
set(MV_OPENGL ${MV_OPENGL})
set(MV_DEMOS ${MV_OPENGL})

# various settings for third party libs
set(GLFW_BUILD_DOCS OFF)
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_definitions(-D_USE_MATH_DEFINES)

add_subdirectory ("Dependencies/glfw")
add_subdirectory ("Dependencies/glm")
add_subdirectory ("Dependencies/assimp")

if(MV_VULKAN)
	add_subdirectory ("Marvel_vulkan")
	if(MV_DEMOS)
		add_subdirectory ("Demo_Vulkan")
	endif()
endif()

if(MV_OPENGL)
	add_subdirectory ("Marvel_opengl")
	if(MV_DEMOS)
		add_subdirectory ("Demo_OpenGL")
	endif()
endif()

if(WIN32)

	add_definitions(-DGLFW_EXPOSE_NATIVE_WIN32)

	if(MV_D3D11)
		add_subdirectory ("Marvel_d3d11")
		
		if(MV_DEMOS)
			add_subdirectory ("Demo_Directx11")
		endif()

	endif()
	
	if(MV_D3D11)
		if(MV_DEMOS)
			add_subdirectory ("Demo_Directx12")
		endif()
	endif()

endif()