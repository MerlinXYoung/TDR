if(WIN32)
	find_path(TDR_INCLUDE_DIR NAMES platform/tdr_platform.h
		PATHS
		$ENV{PROGRAMFILES}/TLibC/include
	   )
else()
	find_path(TDR_INCLUDE_DIR NAMES platform/tdr_platform.h
		PATHS
		/usr/local/tlibc/include
	   )
endif(WIN32)

if(WIN32)
	find_library(TDR_LIBRARIES NAMES tlibc
		PATHS 
		$ENV{PROGRAMFILES}/TLibC/lib
	   )
else()
	find_library(TDR_LIBRARIES NAMES tlibc
		PATHS 
		/usr/local/tlibc/lib
	   )
endif(WIN32)

mark_as_advanced(LIBC_INCLUDE_DIR TDR_LIBRARIES)
