cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME dameEmu)
project(${PROJECT_NAME})

set(GUI_SRC
	GUI/AppMain.h
	GUI/AppMain.cpp
	GUI/MainCanvas.h
	GUI/MainCanvas.cpp
	GUI/Add-ons/imfilebrowser.h
)

set(Cartridge_SRC
	Cartridge/Cartridge.h
	Cartridge/Cartridge.cpp
	Cartridge/Mapper.h
	Cartridge/Mapper.cpp
	Cartridge/MBC1.cpp
)

set(SRC
	${GUI_SRC}
	${Cartridge_SRC}
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
    Disassembler.h
    Disassembler.cpp
    InstructionSet.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GUI_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Cartridge_SRC})

add_executable(${PROJECT_NAME} WIN32 ${SRC})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if(MSVC)
	target_link_options(${PROJECT_NAME} PUBLIC "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
	set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPEDENCIES})

find_package(ImGui-SFML REQUIRED)
target_link_libraries(${PROJECT_NAME} ImGui-SFML::ImGui-SFML)