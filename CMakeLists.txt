cmake_minimum_required (VERSION 3.13)

project ("GraphicsAPIs")

# various settings
set(GLFW_BUILD_DOCS OFF)
add_definitions(-D_USE_MATH_DEFINES)

add_subdirectory ("Marvel_d3d11")
add_subdirectory ("Marvel_opengl")
add_subdirectory ("Directx11_ht")
add_subdirectory ("Vulkan_ht")
add_subdirectory ("OpenGL_ht")
add_subdirectory ("Dependencies/glfw")
add_subdirectory ("Dependencies/glm")