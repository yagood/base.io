cmake_minimum_required(VERSION 2.6)

project( base.io )

set(OUTPUT_ELF "${CMAKE_PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall")

set(SOURCE_FILES
    src/game/game.cpp
    src/game/map.cpp
    src/entities/entity.cpp
    src/entities/cell.cpp
    src/entities/food.cpp
    src/modules/buffer.cpp
    src/modules/logger.cpp
    src/modules/utils.cpp
    src/modules/vec2.cpp
    src/modules/quad_tree.cpp
    src/modules/commands.cpp
    src/network/server.cpp
    src/network/packet.cpp
    src/network/packet_handler.cpp
    src/play/player.cpp
    src/settings.cpp
    src/main.cpp
  )

include_directories(src)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)
link_libraries(uWs uv z ssl)

add_executable(${OUTPUT_ELF} ${SOURCE_FILES})

target_link_libraries(${OUTPUT_ELF} pthread)
