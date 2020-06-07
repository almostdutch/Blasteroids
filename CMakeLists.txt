cmake_minimum_required (VERSION 3.10.2)
project (blasteroids)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
include_directories(${CMAKE_SOURCE_DIR}/include)
set(ALLEGRO_LIBS allegro allegro_primitives allegro_color allegro_image allegro_font m)

add_executable (${PROJECT_NAME} "${SOURCES}")
target_link_libraries (${PROJECT_NAME} ${ALLEGRO_LIBS}) 
