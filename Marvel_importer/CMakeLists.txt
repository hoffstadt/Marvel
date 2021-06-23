cmake_minimum_required (VERSION 3.13)

add_library(marvel_importer)
set_property(TARGET marvel_importer PROPERTY CXX_STANDARD 17)

target_sources(marvel_importer

	PRIVATE

		"src/mvMarvelImporter.cpp"

)

target_include_directories(marvel_importer

	PRIVATE

		"src"
		"../Dependencies/json/"

)
