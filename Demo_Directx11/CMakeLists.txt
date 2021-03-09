cmake_minimum_required (VERSION 3.13)

add_executable (demo_d3d11)

set_property(TARGET demo_d3d11 PROPERTY CXX_STANDARD 17)

target_sources(demo_d3d11

	PRIVATE

		"main.cpp"

)

target_link_libraries(demo_d3d11

	PRIVATE
		d3d11
		d3dcompiler
)
