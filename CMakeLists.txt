SET(PNAME "TDR")
project(${PNAME} C)

cmake_minimum_required(VERSION 2.6)

set(LIBNAME tdr)
set(EXECNAME tdrc)

IF(WIN32)
	SET(CMAKE_C_FLAGS_DEBUG "/Od /Z7 /W4 /MDd")
	SET(CMAKE_C_FLAGS_RELEASE "/O2 /W4 /MD")
ELSE()
	SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
	SET(CMAKE_C_FLAGS_RELEASE "-O3 -Wall")
	add_definitions(-DLUA_COMPAT_ALL -DLUA_USE_POSIX)
ENDIF()

SET(BUILDTYPE_DOCSTRING "Choose the type of build, options are: Debug(${CMAKE_C_FLAGS_DEBUG}) Release(${CMAKE_C_FLAGS_RELEASE})")
IF(WIN32)
	SET(CMAKE_BUILD_TYPE "Release" CACHE STRING ${BUILDTYPE_DOCSTRING})
ELSE()
	SET(CMAKE_BUILD_TYPE "Release" CACHE STRING ${BUILDTYPE_DOCSTRING})
	SET(CMAKE_INSTALL_PREFIX "/usr/local/${PONAME}" CACHE STRING "" FORCE)
ENDIF()

SET(CMAKE_MODULE_PATH "${${PNAME}_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}" )
set(DIR_HOME ${${PNAME}_SOURCE_DIR})
set(BUILD_HOME ${${PNAME}_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tutorial/cmake/Modules;${CMAKE_MODULE_PATH})
find_package(MySQL REQUIRED)
include_directories(${MYSQL_INCLUDE_DIR})


add_subdirectory(tdr)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tdr)
add_subdirectory(tdrc)

install(DIRECTORY lua DESTINATION ".")
