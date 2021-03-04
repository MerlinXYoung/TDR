PROJECT(Example)
cmake_minimum_required(VERSION 2.8)

FILE(GLOB FILE_LIST tsf4g_example/source/*.c)

SET(CMAKE_MODULE_PATH "${Example_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}" )

# find_package(TDR REQUIRED)
set(TDR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/../build/tdrc/tdrc)
# find_package(TLibC REQUIRED)
set(TDR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../tdr)
set(TDR_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../build/tdr/libtdr.a)

find_package(MySQL REQUIRED)
find_package(ZLIB REQUIRED)

IF(WIN32)
	SET(CMAKE_C_FLAGS_DEBUG "/Od /Z7 /W4 /MDd")
	SET(CMAKE_C_FLAGS_RELEASE "/O2 /W4 /MD")
ELSE()
	SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
	SET(CMAKE_C_FLAGS_RELEASE "-O3 -Wall")
ENDIF()

SET(BUILDTYPE_DOCSTRING "Choose the type of build, options are: Debug(${CMAKE_C_FLAGS_DEBUG}) Release(${CMAKE_C_FLAGS_RELEASE})")
IF(WIN32)
    SET(CMAKE_BUILD_TYPE "Release" CACHE STRING ${BUILDTYPE_DOCSTRING})
ELSE()
    SET(CMAKE_BUILD_TYPE "Release" CACHE STRING ${BUILDTYPE_DOCSTRING})
    SET(CMAKE_INSTALL_PREFIX "/usr/local/${PNAME}" CACHE STRING "" FORCE)
ENDIF()

set(GEN_PATH ${Example_BINARY_DIR}/gen)
INCLUDE_DIRECTORIES(
	${TDR_INCLUDE_DIR}
	${GEN_PATH}/
	${ZLIB_INCLUDE_DIRS}
	${MYSQL_INCLUDE_DIR}
	)

set(TDR_OUTPUTS 
	${GEN_PATH}/common_types.h ${GEN_PATH}/common_reader.c ${GEN_PATH}/common_reader.h 
	${GEN_PATH}/common_writer.c ${GEN_PATH}/common_writer.h
	${GEN_PATH}/protocol_types.h
	${GEN_PATH}/protocol_reader.c ${GEN_PATH}/protocol_reader.h 
	${GEN_PATH}/protocol_writer.c ${GEN_PATH}/protocol_writer.h
	${GEN_PATH}/sql_types.h
	${GEN_PATH}/sql_reader.c ${GEN_PATH}/sql_reader.h 
	${GEN_PATH}/sql_writer.c ${GEN_PATH}/sql_writer.h
	# ${GEN_PATH}/tconnd_inc.xml ${GEN_PATH}/tconnd_instance1.xml.inc
	# ${GEN_PATH}/item.xlsx.xml ${GEN_PATH}/item.xlsx
	${GEN_PATH}/sql_tables.sql
	)

add_custom_command(OUTPUT ${TDR_OUTPUTS}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${GEN_PATH}
	COMMAND ${TDR_EXECUTABLE} -I${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr -o${GEN_PATH} -gtypes_h ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/protocol.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/common/common.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr -o${GEN_PATH} -greader_h ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/protocol.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/common/common.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr -o${GEN_PATH} -greader_c ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/protocol.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/common/common.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr -o${GEN_PATH} -gwriter_h ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/protocol.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/common/common.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr -o${GEN_PATH} -gwriter_c ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/protocol.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/common/common.tdr ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr -o ${GEN_PATH} -g sql ${CMAKE_CURRENT_SOURCE_DIR}/tsf4g_example/tdr/sql.tdr
	# COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tconnd_inc.xml ${GEN_PATH}
	# COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tconnd_instance1.xml.inc ${GEN_PATH}
	# COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/item.xlsx ${GEN_PATH}
	DEPENDS ${TDR_EXECUTABLE}
)

ADD_EXECUTABLE(Example ${FILE_LIST} ${TDR_OUTPUTS})
TARGET_LINK_LIBRARIES(Example ${TDR_LIBRARIES} ${ZLIB_LIBRARIES} ${MYSQL_LIBRARIES} )
