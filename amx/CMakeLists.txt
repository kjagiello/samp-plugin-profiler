include(AmxConfig)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} APPEND PROPERTY INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)  
endif()

add_library(amx STATIC
	amx.h
	amxaux.c
	amxaux.h
	amxdbg.c
	amxdbg.h
	getch.h
	osdefs.h
	sclinux.h
)