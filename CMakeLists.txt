cmake_minimum_required (VERSION 3.13)

project ("GraphicsAPIs")

# various settings
set(GLFW_BUILD_DOCS OFF)
add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DGLFW_EXPOSE_NATIVE_WIN32)

add_subdirectory ("Marvel_d3d11")
add_subdirectory ("Marvel_opengl")
add_subdirectory ("Demo_Directx11")
add_subdirectory ("Demo_Directx12")
add_subdirectory ("Demo_Vulkan")
add_subdirectory ("Demo_OpenGL")
add_subdirectory ("Dependencies/glfw")
add_subdirectory ("Dependencies/glm")