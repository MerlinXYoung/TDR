set(GEN_PATH ${SOURCE}/gen)
set(GEN_INCLUDE ${GEN_PATH}/include)
set(GEN_SOURCE ${GEN_PATH}/source)


FILE(GLOB FILE_LIST *.h *.c)

FILE(GLOB PLATFORM_FILE_LIST platform/*.h platform/*.c)
source_group("platform" FILES ${PLATFORM_FILE_LIST})

FILE(GLOB PROTOCOL_FILE_LIST protocol/*.h protocol/*.c)
source_group("protocol" FILES ${PROTOCOL_FILE_LIST})

set(GEN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/protocol/gen)
FILE(GLOB PROTOCOL_FLEX_FIELS protocol/*.re)

foreach(PFLEX ${PROTOCOL_FLEX_FIELS})
	string(REGEX REPLACE ".*/" "" base_name ${PFLEX})
	string(REGEX REPLACE "\\.re$" ".h" TMP_HEADER ${base_name})
	string(REGEX REPLACE "\\.re$" ".c" TMP_SOURCE ${base_name})
	LIST(APPEND PROTOCOL_GEN_LIST ${GEN_PATH}/${TMP_SOURCE})
	add_custom_command(OUTPUT ${GEN_PATH}/${TMP_SOURCE} ${GEN_PATH}/${TMP_HEADER}
		COMMAND mkdir -p ${GEN_PATH}
		COMMAND re2c -c -F -t${GEN_PATH}/${TMP_HEADER} -o${GEN_PATH}/${TMP_SOURCE} ${PFLEX}
		DEPENDS ${PFLEX}
	)
endforeach()


add_library(${LIBNAME} ${FILE_LIST} ${PLATFORM_FILE_LIST} ${PROTOCOL_FILE_LIST} ${PROTOCOL_GEN_LIST})

INSTALL(TARGETS ${LIBNAME} DESTINATION "lib")
install_files("include" extension *.h)



