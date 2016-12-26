# Find osmpbf header and library - 20161226
# 
# This module defines the following uncached variables:
#  OSMPBF_FOUND, if false, do not try to use osmpbf
#  OSMPBF_INCLUDE_DIR, where to find osmpbf/osmpbf.h
#  OSMPBF_LIBRARIES, the libraries to link against to use the libprotobuf
#  OSMPBF_LIBRARY_DIRS, the directory where the libprotobuf library is found.
#
#  If installed in a 'non-standard' location can use OSMPBF_ROOT
#  to assist in the finding.

find_path(OSMPBF_INCLUDE_DIR osmpbf/osmpbf.h
	HINTS ${OSMPBF_ROOT} $ENV{OSMPBF_ROOT}
	PATHS /usr/local/include /usr/include 
	PATH_SUFFIXES include
	)
    
if(MSVC)
    find_library(OSMPBF_LIB_DBG
        NAMES osmpbfd
        HINTS ${OSMPBF_ROOT} $ENV{OSMPBF_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
    find_library(OSMPBF_LIB_REL
        NAMES osmpbf
        HINTS ${OSMPBF_ROOT} $ENV{OSMPBF_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
    if (OSMPBF_LIB_DBG AND OSMPBF_LIB_REL)
        set(OSMPBF_LIBRARY
            optimized ${OSMPBF_LIB_REL}
            debug ${OSMPBF_LIB_DBG}
            )
    elseif (OSMPBF_LIB_REL)
        set(OSMPBF_LIBRARY ${OSMPBF_LIB_REL})
    endif ()
else()
    find_library(OSMPBF_LIBRARY
        NAMES osmpbf
        HINTS ${OSMPBF_ROOT} $ENV{OSMPBF_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
endif()

if(OSMPBF_INCLUDE_DIR AND OSMPBF_LIBRARY)
	# Set uncached variables as per standard.
   	set(OSMPBF_FOUND ON)
	set(OSMPBF_LIBRARIES ${OSMPBF_LIBRARY})
    if (MSVC)
        get_filename_component(OSMPBF_LIBRARY_DIRS ${OSMPBF_LIB_REL} PATH)
    else ()
        get_filename_component(OSMPBF_LIBRARY_DIRS ${OSMPBF_LIBRARY} PATH)
    endif ()
endif()

if(OSMPBF_FOUND)
	if(NOT OSMPBF_FIND_QUIETLY)
		message(STATUS "Found osmpbf inc ${OSMPBF_INCLUDE_DIR}, lib ${OSMPBF_LIBRARIES}.")
	endif()
else()
	set(msg "osmpbf:")
	if(OSMPBF_INCLUDE_DIR)
		set(msg "${msg} found header osmpbf/osmpbf.h ${OSMPBF_INCLUDE_DIR}")
	else()
		set(msg "${msg} header osmpbf/osmpbf.h not found")
	endif()
	if(OSMPBF_LIBRARY)
		set(msg "${msg} found library ${OSMPBF_LIBRARY}")
	else()
		set(msg "${msg} libraries libprotobuf not found")
	endif()
	if(OSMPBF_FIND_REQUIRED)
		message(FATAL_ERROR "${msg}")
	else()
		if(NOT OSMPBF_FIND_QUIETLY)
			message(STATUS "${msg}")
		endif()
	endif()
endif()

# eof
