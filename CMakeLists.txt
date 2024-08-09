cmake_minimum_required(VERSION 3.25)

project(games_idp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SHARED_LINKER_FLAGS "-m64")


set(SRC 
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/idp.cpp
	)

add_executable(sdl_test ${SRC})

add_subdirectory(vendor/SDL2-2.30.3 EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL2_image-2.8.2 EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL2_ttf-2.22.0 EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL2_mixer-2.8.0 EXCLUDE_FROM_ALL)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(sdl_test PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(sdl_test PRIVATE SDL2::SDL2)
target_link_libraries(sdl_test PRIVATE SDL2_image::SDL2_image)
target_link_libraries(sdl_test PRIVATE SDL2_ttf::SDL2_ttf)
target_link_libraries(sdl_test PRIVATE SDL2_mixer::SDL2_mixer)

target_include_directories(sdl_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(sdl_test PRIVATE ${PROJECT_SOURCE_DIR}/vendor/headeronly)
target_precompile_headers(sdl_test PRIVATE ${PROJECT_SOURCE_DIR}/include/MSDL.hpp)
