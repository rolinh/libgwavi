cmake_minimum_required(VERSION 3.0)

project(libgwavi 
	LANGUAGES C
	HOMEPAGE_URL https://github.com/rolinh/libgwavi
	DESCRIPTION "libgwavi is a tiny C library aimed at creating AVI files"
	VERSION 1.0.0
)

add_compile_options(-Wall -pedantic -D_FILE_OFFSET_BITS=64)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_library(libgwavi SHARED
	src/avi-utils.c
	src/fileio.c
	src/gwavi.c
)

include(GNUInstallDirs)

target_include_directories(libgwavi PUBLIC inc)

set_target_properties(libgwavi PROPERTIES VERSION ${PROJECT_VERSION} PUBLIC_HEADER inc/gwavi.h)

configure_file(libgwavi.pc.in libgwavi.pc @ONLY) 

install(TARGETS libgwavi
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
	)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libgwavi.pc DESTINATION lib/pkgconfig)
