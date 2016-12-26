# Find shapelib header and library.
# 
# This module defines the following uncached variables:
#  SHAPELIB_FOUND, if false, do not try to use shapelib.
#  SHAPELIB_INCLUDE_DIR, where to find shapefil.h.
#  SHAPELIB_LIBRARIES, the libraries to link against to use the shapelib
#  library.
#  SHAPELIB_LIBRARY_DIRS, the directory where the shapelib library is found.
#
#  If installed in a 'non-standard' location can use SHAPELIB_ROOT
#  to assist in the finding.

find_path(SHAPELIB_INCLUDE_DIR shapefil.h
	HINTS ${SHAPELIB_ROOT} $ENV{SHAPELIB_ROOT}
	PATHS /usr/local/include /usr/include 
	PATH_SUFFIXES include
	)
    
if(MSVC)
    find_library(SHAPELIB_LIB_DBG
        NAMES shpd shapelibd
        HINTS ${SHAPELIB_ROOT} $ENV{SHAPELIB_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
    find_library(SHAPELIB_LIB_REL
        NAMES shp shapelib
        HINTS ${SHAPELIB_ROOT} $ENV{SHAPELIB_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
    if (SHAPELIB_LIB_DBG AND SHAPELIB_LIB_REL)
        set(SHAPELIB_LIBRARY
            optimized ${SHAPELIB_LIB_REL}
            debug ${SHAPELIB_LIB_DBG}
            )
    elseif (SHAPELIB_LIB_REL)
        set(SHAPELIB_LIBRARY ${SHAPELIB_LIB_REL})
    endif ()
else()
    find_library(SHAPELIB_LIBRARY
        NAMES shp shapelib
        HINTS ${SHAPELIB_ROOT} $ENV{SHAPELIB_ROOT}
        PATHS /usr/local/lib /usr/lib
        PATH_SUFFIXES lib
        )
endif()

if(SHAPELIB_INCLUDE_DIR AND SHAPELIB_LIBRARY)
	# Set uncached variables as per standard.
   	set(SHAPELIB_FOUND ON)
	set(SHAPELIB_LIBRARIES ${SHAPELIB_LIBRARY})
    if (MSVC)
        get_filename_component(SHAPELIB_LIBRARY_DIRS ${SHAPELIB_LIB_REL} PATH)
    else ()
        get_filename_component(SHAPELIB_LIBRARY_DIRS ${SHAPELIB_LIBRARY} PATH)
    endif ()
endif()

if(SHAPELIB_FOUND)
	if(NOT SHAPELIB_FIND_QUIETLY)
		message(STATUS "Found shapelib inc ${SHAPELIB_INCLUDE_DIR}, lib ${SHAPELIB_LIBRARIES}.")
	endif()
else()
	set(msg "FindShapelib:")
	if(SHAPELIB_INCLUDE_DIR)
		set(msg "${msg} found header shapefil.h ${SHAPELIB_INCLUDE_DIR}")
	else()
		set(msg "${msg} header shapefil.h not found")
	endif()
	if(SHAPELIB_LIBRARY)
		set(msg "${msg} found library ${SHAPELIB_LIBRARY}")
	else()
		set(msg "${msg} libraries shp shapelib not found")
	endif()
	if(SHAPELIB_FIND_REQUIRED)
		message(FATAL_ERROR "${msg}")
	else()
		if(NOT SHAPELIB_FIND_QUIETLY)
			message(STATUS "${msg}")
		endif()
	endif()
endif()

# eof
