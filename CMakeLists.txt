cmake_minimum_required(VERSION 3.10)
#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(DameEmu)
set(SRC
    ${SRC}
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/DameEmu.h
    ${CMAKE_SOURCE_DIR}/DameEmu.cpp
	${CMAKE_SOURCE_DIR}/CPU.h
    ${CMAKE_SOURCE_DIR}/CPU.cpp
    ${CMAKE_SOURCE_DIR}/instructions.cpp
    ${CMAKE_SOURCE_DIR}/cb_instructions.cpp
    ${CMAKE_SOURCE_DIR}/MMU.h
    ${CMAKE_SOURCE_DIR}/MMU.cpp
    ${CMAKE_SOURCE_DIR}/PPU.h
    ${CMAKE_SOURCE_DIR}/PPU.cpp
)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("DEBUG BUILD")
    add_definitions(-D_DEBUG)
endif()

add_executable(DameEmu ${SRC})

find_package(SFML 2 REQUIRED graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(DameEmu ${SFML_LIBRARIES})
    target_link_libraries(DameEmu
    sfml-system
    sfml-window
    sfml-graphics
    )
endif()
 
