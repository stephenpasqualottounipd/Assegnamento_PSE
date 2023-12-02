# ./lib/train/CMakeLists.txt

cmake_minimum_required( VERSION 3.10.2)

project( assegnamento
         VERSION 1.0
	 LANGUAGES CXX
         DESCRIPTION "Libreria per la gestione del boid" )

add_library( boid SHARED src/boid.cpp)

target_include_directories( boid
    PUBLIC include )

add_library( LII::boid ALIAS boid )
