cmake_minimum_required(VERSION 3.14)
project(Packer)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

find_package(SDL2 REQUIRED)

include_directories("../")
add_executable(Packer main.c ../src/Packer.c)
# this is here cuz sometimes mingw64 just doesnt like me
if (NOT DEFINED SDL2_LIBRARIES)
	set(SDL2_LIBRARIES SDL2)
endif()
target_link_libraries(${PROJECT_NAME} m ${SDL2_LIBRARIES})