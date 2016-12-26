# Find Protobuf header and library - 20161226
# 
# This module defines the following uncached variables:
#  PROTOBUF_FOUND, if false, do not try to use libprotobuf
#  PROTOBUF_INCLUDE_DIR, where to find google/protobuf/stubs/common.h
#  PROTOBUF_LIBRARIES, the libraries to link against to use the libprotobuf
#  PROTOBUF_LIBRARY_DIRS, the directory where the libprotobuf library is found.
#
#  If installed in a 'non-standard' location can use PROTOBUF_ROOT
#  to assist in the finding.

find_path(PROTOBUF_INCLUDE_DIR google/protobuf/stubs/common.h
	HINTS ${PROTOBUF_ROOT} $ENV{PROTOBUF_ROOT}
	PATHS /usr/local/include /usr/include 
	PATH_SUFFIXES include
	)
    
if(MSVC)
    find_library(PROTOBUF_LIB_DBG
        NAMES libprotobufd
        HINTS ${PROTOBUF_ROOT} $ENV{PROTOBUF_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
    find_library(PROTOBUF_LIB_REL
        NAMES libprotobuf
        HINTS ${PROTOBUF_ROOT} $ENV{PROTOBUF_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
    if (PROTOBUF_LIB_DBG AND PROTOBUF_LIB_REL)
        set(PROTOBUF_LIBRARY
            optimized ${PROTOBUF_LIB_REL}
            debug ${PROTOBUF_LIB_DBG}
            )
    elseif (PROTOBUF_LIB_REL)
        set(PROTOBUF_LIBRARY ${PROTOBUF_LIB_REL})
    endif ()
else()
    find_library(PROTOBUF_LIBRARY
        NAMES libprotobuf protobuf
        HINTS ${PROTOBUF_ROOT} $ENV{PROTOBUF_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
endif()

if(PROTOBUF_INCLUDE_DIR AND PROTOBUF_LIBRARY)
	# Set uncached variables as per standard.
   	set(PROTOBUF_FOUND ON)
	set(PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARY})
    if (MSVC)
        get_filename_component(PROTOBUF_LIBRARY_DIRS ${PROTOBUF_LIB_REL} PATH)
    else ()
        get_filename_component(PROTOBUF_LIBRARY_DIRS ${PROTOBUF_LIBRARY} PATH)
    endif ()
endif()

if(PROTOBUF_FOUND)
	if(NOT PROTOBUF_FIND_QUIETLY)
		message(STATUS "Found protobuf inc ${PROTOBUF_INCLUDE_DIR}, lib ${PROTOBUF_LIBRARIES}.")
	endif()
else()
	set(msg "libprotobuf:")
	if(PROTOBUF_INCLUDE_DIR)
		set(msg "${msg} found header google/protobuf/stubs/common.h ${PROTOBUF_INCLUDE_DIR}")
	else()
		set(msg "${msg} header google/protobuf/stubs/common.h not found")
	endif()
	if(PROTOBUF_LIBRARY)
		set(msg "${msg} found library ${PROTOBUF_LIBRARY}")
	else()
		set(msg "${msg} libraries libprotobuf not found")
	endif()
	if(PROTOBUF_FIND_REQUIRED)
		message(FATAL_ERROR "${msg}")
	else()
		if(NOT PROTOBUF_FIND_QUIETLY)
			message(STATUS "${msg}")
		endif()
	endif()
endif()

# eof
