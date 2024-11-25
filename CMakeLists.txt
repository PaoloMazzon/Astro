cmake_minimum_required(VERSION 3.14)
project(Astro)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -ffast-math -funroll-loops")

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)

add_subdirectory("Packer")

# All source files are located in the VK2D folder
file(GLOB C_FILES Vulkan2D/VK2D/*.c src/*.c wren/src/optional/*.c wren/src/vm/*.c)
file(GLOB H_FILES Vulkan2D/VK2D/*.h)
file(GLOB CPP_FILES src/*.cpp)
set(VMA_FILES Vulkan2D/VulkanMemoryAllocator/src/VmaUsage.cpp)
set(STEAM_LIBS "C:\\steamworks_sdk_161\\sdk\\redistributable_bin\\win64\\steam_api64.lib")
set(STEAM_INCLUDE "C:\\steamworks_sdk_161\\sdk\\public\\")

include_directories(./ ${STEAM_INCLUDE} Vulkan2D/ src/ ${SDL2_INCLUDE_DIR} ${Vulkan_INCLUDE_DIRS} wren/src/include/ wren/src/vm/ wren/src/optional/)
if (WIN32)
	add_executable(Astro main.c icon.rc ${VMA_FILES} ${C_FILES} ${CPP_FILES} ${H_FILES})
else()
	add_executable(Astro main.c ${VMA_FILES} ${C_FILES} ${CPP_FILES} ${H_FILES})
endif()

# Accounts for msys2/mingw64 compilers
if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU" AND WIN32)
	set(SDL2_LIBRARIES mingw32 SDL2main SDL2 m)
endif()

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${STEAM_LIBS} Vulkan::Vulkan)