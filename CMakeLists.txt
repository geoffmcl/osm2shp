# 
# CMakeLists.txt, generated gencmake.pl, on 2016/12/16 20:26:39
cmake_minimum_required( VERSION 2.8.8 )

# CMakeScripts or use the ones that come by default with CMake.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)     # ${CMAKE_MODULE_PATH})

project( osm2shp )

# The version number.
set( osm2shp_MAJOR 3 )
set( osm2shp_MINOR 0 )
set( osm2shp_POINT 0 )


# Allow developer to select is Dynamic or static library built
set( LIB_TYPE STATIC )  # set default static
option( BUILD_SHARED_LIB "Set ON to build shared Library (DLL)" OFF )

if(CMAKE_COMPILER_IS_GNUCXX)
    set( WARNING_FLAGS -Wall )
endif(CMAKE_COMPILER_IS_GNUCXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
   set( WARNING_FLAGS "-Wall -Wno-overloaded-virtual" )
endif() 

if(WIN32 AND MSVC)
    # turn off various warnings - none needed in this compile
    set(WARNING_FLAGS "${WARNING_FLAGS} /wd4996")
    # foreach(warning 4244 4251 4267 4275 4290 4786 4305)
    #     set(WARNING_FLAGS "${WARNING_FLAGS} /wd${warning}")
    # endforeach(warning)
    set( MSVC_FLAGS "-DNOMINMAX -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D__CRT_NONSTDC_NO_WARNINGS" )
    # if (${MSVC_VERSION} EQUAL 1600)
    #    set( MSVC_LD_FLAGS "/FORCE:MULTIPLE" )
    # endif (${MSVC_VERSION} EQUAL 1600)
    #set( NOMINMAX 1 )
    # to distinguish between debug and release libs
    set( CMAKE_DEBUG_POSTFIX "d" )
else()
    # items for unix
endif()

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LD_FLAGS}" )

#  SHAPELIB_FOUND, if false, do not try to use shapelib.
#  SHAPELIB_INCLUDE_DIR, where to find shapefil.h.
#  SHAPELIB_LIBRARIES, the libraries to link against to use the shapelib library.
find_package(Shapelib)
if (SHAPELIB_FOUND)
    message(STATUS "*** Shapelib found inc ${SHAPELIB_INCLUDE_DIR}, lib ${SHAPELIB_LIBRARIES}")
    include_directories( ${SHAPELIB_INCLUDE_DIR} )
    list(APPEND add_LIBS ${SHAPELIB_LIBRARIES} )
else ()
    message(FATAL_ERROR "*** Shapelib NOT FOUND *** FIX ME ***")
endif ()

#   Boost_FOUND            - True if headers and requested libraries were found
#   Boost_INCLUDE_DIRS     - Boost include directories
find_package( Boost )
if (Boost_FOUND)
    message(STATUS "*** Boost found inc ${Boost_INCLUDE_DIRS}" )
    include_directories( ${Boost_INCLUDE_DIRS} )
else ()
    message(FATAL_ERROR "*** Boost NOT FOUND!" )
endif ()

include_directories( X:/osmium/include )

#   EXPAT_INCLUDE_DIRS - where to find expat.h, etc.
#   EXPAT_LIBRARIES    - List of libraries when using expat.
#   EXPAT_FOUND        - True if expat found.
find_package( EXPAT )
if (EXPAT_FOUND)
    message(STATUS "*** EXPAT found inc ${EXPAT_INCLUDE_DIRS}, lib ${EXPAT_LIBRARIES}" )
    include_directories( ${EXPAT_INCLUDE_DIRS} )
    list(APPEND add_LIBS ${EXPAT_LIBRARIES} )
    add_definitions( -DXML_STATIC )
else ()
    message(FATAL_ERROR "*** EXPAT NOT FOUND!" )
endif ()

# SQLITE3_FOUND
# SQLITE3_INCLUDE_DIR
# SQLITE3_LIBRARY
find_package( SQLite3 )
if (SQLITE3_FOUND)
    message(STATUS "*** SQLite3 found inc ${SQLITE3_INCLUDE_DIR}, lib ${SQLITE3_LIBRARY}" )
    include_directories( ${SQLITE3_INCLUDE_DIR} )
    list(APPEND add_LIBS ${SQLITE3_LIBRARY} )
else ()
    message(FATAL_ERROR "*** SQLite3 NOT FOUND!" )
endif ()

# PROTOBUF_FOUND
# PROTOBUF_INCLUDE_DIRS
# PROTOBUF_LIBRARIES
find_package( Protobuf )
if (PROTOBUF_FOUND)
    message(STATUS "*** Protobuf found inc ${PROTOBUF_INCLUDE_DIRS}, lib ${PROTOBUF_LIBRARIES}" )
    include_directories( ${PROTOBUF_INCLUDE_DIRS} )
    list(APPEND add_LIBS ${PROTOBUF_LIBRARIES} )
else ()
    message(FATAL_ERROR "*** Protobuf NOT FOUND!" )
endif ()    

# This module defines the following uncached variables:
#  OSMPBF_FOUND, if false, do not try to use osmpbf
#  OSMPBF_INCLUDE_DIR, where to find osmpbf/osmpbf.h
#  OSMPBF_LIBRARIES, the libraries to link against to use the osmpbf
#  OSMPBF_LIBRARY_DIRS, the directory where the libprotobuf library is found.
find_package( OSMpbf )
if (OSMPBF_FOUND)
    message(STATUS "*** osmpbf found inc ${OSMPBF_INCLUDE_DIR}, lib ${OSMPBF_LIBRARIES}" )
    include_directories( ${OSMPBF_INCLUDE_DIR} )
    list(APPEND add_LIBS ${OSMPBF_LIBRARIES} )
else ()
    message(FATAL_ERROR "*** osmpbf NOT FOUND!" )
endif ()    

#   ZLIB_INCLUDE_DIRS   - where to find zlib.h, etc.
#   ZLIB_LIBRARIES      - List of libraries when using zlib.
#   ZLIB_FOUND          - True if zlib found.
find_package( ZLIB )
if (ZLIB_FOUND)
    message(STATUS "*** ZLIB found inc ${ZLIB_INCLUDE_DIRS}, lib ${ZLIB_LIBRARIES}" )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    list(APPEND add_LIBS ${ZLIB_LIBRARIES} )
else ()
    message(FATAL_ERROR "*** ZLIB NOT FOUND!" )
endif ()    

# configuration file, if needed
# configure_file( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h )
# add_definitions( -DHAVE_CONFIG_H )
# include_directories( ${CMAKE_BINARY_DIR} )

if(BUILD_SHARED_LIB)
   set(LIB_TYPE SHARED)
   message(STATUS "*** Building DLL library ${LIB_TYPE}")
else()
   message(STATUS "*** Option BUILD_SHARED_LIB is OFF ${LIB_TYPE}")
endif()
# Total libs 0, exes 1
#################################################
##### LIBRARIES 0 #####
#################################################
# deal with install, if any...
#install( TARGETS ${name}
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib )
#install(FILES ${${name}_HDRS} DESTINATION include)
#################################################
##### EXECUTABLE 1 #####
#################################################

# osm2shp EXECUTABLE from [G:\F\osm2shp],
# have 4 C/C++ sources, 4 headers
set(name osm2shp)
set(dir osm)
set(${name}_SRCS
    osm2shp.cc
    ${dir}/handler.cc
    ${dir}/point_database.cc
    ${dir}/shapefile.cc
    )
set(${name}_HDRS
    ${dir}/handler.hpp
    ${dir}/layer.hpp
    ${dir}/point_database.hpp
    ${dir}/shapefile.hpp
    )
add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS})
if (add_LIBS)
    target_link_libraries( ${name} ${add_LIBS} )
endif ()
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install, if any...
#install( TARGETS ${name} DESTINATION bin )

##################################################################
######## DEBUG ONLY - Try to look at ALL vars defined
if (LIST_ALL_VARIABLES)
    message(STATUS "******** SHOW ALL VARS *********")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "=== ${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "******** END LIST *********")
endif ()    
##################################################################

# eof
