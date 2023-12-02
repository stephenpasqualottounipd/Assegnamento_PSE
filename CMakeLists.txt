cmake_minimum_required( VERSION 3.10.2 )

project( assegnamento
         VERSION 1.0
         DESCRIPTION "Assegnamento bird-oid object PSE 23/24" )

if (MSVC)
	# warning level 4 and all warnings as errors
	add_compile_options(/W4 /WX)
else()
	# lots of warnings and all warnings as errors
	add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory( lib/boid )

add_subdirectory( app )


