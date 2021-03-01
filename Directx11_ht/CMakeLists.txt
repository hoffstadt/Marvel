cmake_minimum_required (VERSION 3.13)

add_executable (d3d11_ht)

set_property(TARGET d3d11_ht PROPERTY CXX_STANDARD 17)

target_sources(d3d11_ht

	PRIVATE

		"main.cpp"

)

target_link_libraries(d3d11_ht

	PRIVATE
		d3d11
		d3dcompiler
)
