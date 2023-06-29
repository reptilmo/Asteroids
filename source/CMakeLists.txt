cmake_minimum_required(VERSION 3.23)

set(SYSTEM_SRC_FILES
	system/system.h
	system/system.cpp
	system/memory.h
	system/memory_win32.cpp
	system/fileio.h
	system/fileio.cpp
	system/random.h
	system/random.cpp
	system/config.h
	system/config.cpp
)

set(MATH_SRC_FILES
	math/math.h
	math/vector3.h
	math/vector4.h
	math/matrix4.h
	math/transform.h
	math/aabb.h
)

set(RENDERING_SRC_FILES
	rendering/renderer.h
	rendering/renderer.cpp
	rendering/mesh.h
	rendering/mesh.cpp
)

set(AUDIO_SRC_FILES
	audio/sound.h
	audio/sound.cpp
)

set(GAME_SRC_FILES
	game/input.h
	game/input.cpp
	game/global.h
	game/global.cpp
	game/loop.h
	game/loop.cpp
	game/ecs.h
	game/ecs.cpp
	game/quadtree.h
	game/quadtree.cpp
	game/debug.h
	game/debug.cpp
)

add_executable(asteroids
	${SYSTEM_SRC_FILES}
	${MATH_SRC_FILES}
	${RENDERING_SRC_FILES}
	${AUDIO_SRC_FILES}
	${GAME_SRC_FILES}
	main.cpp
)

target_include_directories(asteroids PRIVATE
	${CMAKE_SOURCE_DIR}/source
	${CMAKE_SOURCE_DIR}/3rdparty/glad/include
)

target_link_libraries(asteroids PUBLIC
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_mixer::SDL2_mixer
    glad
)

add_custom_command(
    TARGET asteroids POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/config/asteroids.conf
    $<TARGET_FILE_DIR:asteroids>
    VERBATIM)

if (WIN32)

add_custom_command(
    TARGET asteroids POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2::SDL2>
    $<TARGET_FILE_DIR:asteroids>
    VERBATIM)

add_custom_command(
    TARGET asteroids POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2_mixer::SDL2_mixer>
    $<TARGET_FILE_DIR:asteroids>
    VERBATIM)

endif()
