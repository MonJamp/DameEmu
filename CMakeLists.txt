cmake_minimum_required(VERSION 3.10)
#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


project(DameEmu)

set(GUI_SRC
    GUI/AppMain.h
    GUI/AppMain.cpp
    GUI/MainFrame.h
    GUI/MainFrame.cpp
    GUI/wx_header.h
)
set(SRC
    ${GUI_SRC}
    DameEmu.h
    DameEmu.cpp
	CPU.h
    CPU.cpp
    instructions.cpp
    cb_instructions.cpp
    MMU.h
    MMU.cpp
    PPU.h
    PPU.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GUI_SRC})

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("DEBUG BUILD")
    add_definitions(-D_DEBUG)
endif()

add_executable(DameEmu WIN32 ${SRC})

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
 
 find_package(wxWidgets REQUIRED COMPONENTS core base adv)
 if(wxWidgets_FOUND)
    message(STATUS "Found wxWidgets ${wxWidgets_VERSION_STRING} in ${wxWidgets_USE_FILE}")
    set(wxWidgets_USE_STATIC ON)
    include(${wxWidgets_USE_FILE})
    include_directories(${wxWidgets_INCLUDE_DIRS})
    target_link_libraries(DameEmu ${wxWidgets_LIBRARIES})
endif()