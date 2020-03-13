cmake_minimum_required(VERSION 3.10)
project(DameEmu)


set(GUI_SRC
    GUI/AppMain.h
    GUI/AppMain.cpp
    GUI/DebugFrame.h
    GUI/DebugFrame.cpp
    GUI/RegPanel.h
    GUI/RegPanel.cpp
    GUI/DisasmPanel.h
    GUI/DisasmPanel.cpp
    GUI/DisasmList.h
    GUI/DisasmList.cpp
    GUI/BreakpointList.h
    GUI/BreakpointList.cpp
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

add_executable(DameEmu WIN32 ${SRC})

target_compile_features(DameEmu PUBLIC cxx_std_11)
set_target_properties(DameEmu PROPERTIES CXX_EXTENSIONS OFF)

if(WIN32)
	set_target_properties(DameEmu PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
	add_definitions(
		-DWIN32
		-D__WXMSW__
		-D_WINDOWS
		-D_CRT_SECURE_NO_WARNINGS
	)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -D__WXDEBUG__")
 
 find_package(wxWidgets REQUIRED COMPONENTS core base adv)
 if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(DameEmu ${wxWidgets_LIBRARIES})
endif()
