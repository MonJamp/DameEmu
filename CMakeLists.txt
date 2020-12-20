cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME dameEmu)
project(${PROJECT_NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(GUI_SRC
    GUI/AppMain.h
    GUI/AppMain.cpp
    GUI/Debugger/DebugFrame.h
    GUI/Debugger/DebugFrame.cpp
    GUI/Debugger/RegPanel.h
    GUI/Debugger/RegPanel.cpp
    GUI/Debugger/DisasmPanel.h
    GUI/Debugger/DisasmPanel.cpp
    GUI/Debugger/DisasmList.h
    GUI/Debugger/DisasmList.cpp
    GUI/Debugger/BreakpointList.h
    GUI/Debugger/BreakpointList.cpp
    GUI/Debugger/MemoryBrowser.cpp
    GUI/Debugger/MemoryBrowser.h
    GUI/SfmlCanvas.cpp
    GUI/SfmlCanvas.h
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
    Bus.h
    Bus.cpp
    PPU.h
    PPU.cpp
	Cartridge.h
    Cartridge.cpp
    Disassembler.h
    Disassembler.cpp
    InstructionSet.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GUI_SRC})

add_executable(${PROJECT_NAME} WIN32 ${SRC})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

if(WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
	add_definitions(
		-DWIN32
		-D__WXMSW__
		-D_WINDOWS
		-D_CRT_SECURE_NO_WARNINGS
		-D_SFML_DYNAMIC
	)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})
    target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})

    find_package(X11)
    include_directories(${X11_INCLUDE_DIR})
    link_directories(${X11_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -D__WXDEBUG__")
 
find_package(wxWidgets REQUIRED COMPONENTS core base adv)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    include_directories(${wxWidgets_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
endif()

set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML 2 COMPONENTS window graphics REQUIRED)
if(SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics)
endif()
