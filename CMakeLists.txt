cmake_minimum_required(VERSION 3.14)
project(Astro)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -ffast-math -funroll-loops")

find_package(Vulkan)
find_package(SDL2 REQUIRED)

add_subdirectory("Packer")

# All source files are located in the VK2D folder
file(GLOB C_FILES Vulkan2D/VK2D/*.c src/*.c wren/src/optional/*.c wren/src/vm/*.c)
file(GLOB H_FILES Vulkan2D/VK2D/*.h)
set(VMA_FILES Vulkan2D/VulkanMemoryAllocator/src/VmaUsage.cpp)

include_directories(./ Vulkan2D/ src/ ${SDL2_INCLUDE_DIR} ${Vulkan_INCLUDE_DIRS} wren/src/include/ wren/src/vm/ wren/src/optional/)
if (WIN32)
	add_executable(Astro main.c icon.rc ${VMA_FILES} ${C_FILES} ${H_FILES})
else()
	add_executable(Astro main.c ${VMA_FILES} ${C_FILES} ${H_FILES})
endif()

# Accounts for msys2/mingw64 compilers
if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU" AND WIN32)
	set(SDL2_LIBRARIES mingw32 SDL2main SDL2)
endif()

target_link_libraries(${PROJECT_NAME} m ${SDL2_LIBRARIES} ${Vulkan_LIBRARIES})