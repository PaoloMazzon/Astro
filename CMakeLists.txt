cmake_minimum_required(VERSION 3.14)
project(Astro)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

find_package(Vulkan)
find_package(SDL2 REQUIRED)

# All source files are located in the VK2D folder
file(GLOB C_FILES Vulkan2D/VK2D/*.c src/*.c wren/src/optional/*.c wren/src/vm/*.c)
file(GLOB H_FILES Vulkan2D/VK2D/*.h)
set(VMA_FILES Vulkan2D/VulkanMemoryAllocator/src/vk_mem_alloc.h Vulkan2D/VulkanMemoryAllocator/src/VmaUsage.cpp)

include_directories(./ Vulkan2D/ src/ ${SDL2_INCLUDE_DIR} ${Vulkan_INCLUDE_DIRS} wren/src/include/ wren/src/vm/ wren/src/optional/)
add_executable(Astro main.c ${VMA_FILES} ${C_FILES} ${H_FILES})
# this is here cuz sometimes mingw64 just doesnt like me
if (NOT DEFINED ${SDL2_LIBRARIES})
	set(SDL2_LIBRARIES SDL2)
endif()
target_link_libraries(${PROJECT_NAME} m dsound ${SDL2_LIBRARIES} ${Vulkan_LIBRARIES})