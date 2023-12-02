cmake_minimum_required( VERSION 3.10.2 )

project( app
         VERSION 1.0
         DESCRIPTION "file eseguibile progetto finale" )

add_executable( app src/main.cpp)

target_link_libraries( app
	PRIVATE LII::boid )


