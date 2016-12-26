# Find Sqlite3
# ~~~~~~~~~~~~
# Copyright (c) 2007, Martin Dobias <wonder.sk at gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# CMake module to search for Sqlite3 library
#
# If it's found it sets SQLITE3_FOUND to TRUE
# and following variables are set:
#    SQLITE3_INCLUDE_DIR
#    SQLITE3_LIBRARY


# FIND_PATH and FIND_LIBRARY normally search standard locations
# before the specified paths. To search non-standard paths first,
# FIND_* is invoked first with specified paths and NO_DEFAULT_PATH
# and then again with no specified paths to search the default
# locations. When an earlier FIND_* succeeds, subsequent FIND_*s
# searching for the same item do nothing. 

FIND_PATH(SQLITE3_INCLUDE_DIR sqlite3.h
    PATH_SUFFIXES include 
    HINTS $ENV{SQLITE3DIR}
    PATHS
    ${ADDITIONAL_LIBRARY_PATHS}
  )

if (MSVC)
    FIND_LIBRARY(SQLITE3_LIB_DBG NAMES sqlite3d
        HINTS ${SQLITE3DIR} $ENV{SQLITE3DIR}
        PATH_SUFFIXES lib lib64 libs64 libs libs/Win32 libs/Win64
        PATHS ${ADDITIONAL_LIBRARY_PATHS}
      )
    FIND_LIBRARY(SQLITE3_LIB_REL NAMES sqlite3
        HINTS ${SQLITE3DIR} $ENV{SQLITE3DIR}
        PATH_SUFFIXES lib lib64 libs64 libs libs/Win32 libs/Win64
        PATHS ${ADDITIONAL_LIBRARY_PATHS}
      )
    if (SQLITE3_LIB_DBG AND SQLITE3_LIB_REL)
        set(SQLITE3_LIBRARY
            optimized ${SQLITE3_LIB_REL}
            debug ${SQLITE3_LIB_DBG})
    else ()
        if (SQLITE3_LIB_REL)
            set(SQLITE3_LIBRARY ${SQLITE3_LIB_REL})
        endif ()
    endif ()
else ()
    FIND_LIBRARY(SQLITE3_LIBRARY NAMES sqlite3
        HINTS $ENV{SQLITE3DIR}
        PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
        PATHS ${ADDITIONAL_LIBRARY_PATHS}
      )
endif ()

if (SQLITE3_VERBOSE)
    set(msg "FindSQLite3:")
    if(SQLITE3_INCLUDE_DIR)
        set(msg "${msg} found header sqlite3.h in ${SQLITE3_INCLUDE_DIR}")
    else()
        set(msg "${msg} header sqlite3.h not found")
    endif()
	if(SQLITE3_LIBRARY)
		set(msg "${msg} found library ${SQLITE3_LIBRARY}")
	else()
		set(msg "${msg} libraries shp sqlites lib not found")
	endif()
	message(STATUS "${msg}")
endif ()    

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SQLITE3 DEFAULT_MSG SQLITE3_LIBRARY SQLITE3_INCLUDE_DIR)

# eof
