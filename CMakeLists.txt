cmake_minimum_required(VERSION 3.10)
project(DameEmu)


set(GUI_SRC
    GUI/AppMain.h
    GUI/AppMain.cpp
    GUI/DisasmFrame.h
    GUI/DisasmFrame.cpp
    GUI/DisasmListCtrl.h
    GUI/DisasmListCtrl.cpp
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
	Cartridge.h
    Cartridge.cpp
    Disassembler.h
    Disassembler.cpp
    InstructionSet.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GUI_SRC})

add_executable(DameEmu WIN32 ${SRC})

target_compile_features(DameEmu PUBLIC cxx_std_11)
set_target_properties(DameEmu PROPERTIES CXX_EXTENSIONS OFF)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if(WIN32)
	set_target_properties(DameEmu PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
	add_definitions(
		-DWIN32
		-D__WXMSW__
		-D_WINDOWS
	)
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("DEBUG BUILD")
    add_definitions(
		-D_DEBUG
		-D__WXDEBUG__
		)
endif()
 
 find_package(wxWidgets REQUIRED COMPONENTS core base adv)
 if(wxWidgets_FOUND)
    message(STATUS "Found wxWidgets ${wxWidgets_VERSION_STRING} in ${wxWidgets_USE_FILE}")
    include(${wxWidgets_USE_FILE})
    target_link_libraries(DameEmu ${wxWidgets_LIBRARIES})
endif()
