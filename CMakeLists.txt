cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project( pompeiiDemos VERSION 0.0.1 )
set( pompeiiDemos_VERSION_ABI 1)

# Disable in source building
if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
  message( FATAL "no in source building allowed." )
endif()

list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)

# Default build type is Debug
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif( NOT CMAKE_BUILD_TYPE )

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  add_definitions( -DDEBUG )
endif( )

include(GitExternal)

set( COMMON_PROJECT_DOMAIN maldicion069 )
set( POMPEIIDEMOS_DESCRIPTION "POMPEII DEMOS" )

include(Common)

# Allow warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error")


common_find_package( pompeii REQUIRED SYSTEM )

list( APPEND POMPEIIDEMOS_DEPENDENT_LIBRARIES pompeii )

IF (NOT WIN32)
  link_libraries( -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread )
endif()

common_find_package_post( )

add_subdirectory( demos )

configure_file(
  CMake/glsl2spv.cmake
  ${PROJECT_BINARY_DIR}/${CMAKE_MODULE_INSTALL_PATH}/glsl2spv.cmake
  COPYONLY
  )
install( FILES CMake/glsl2spv.cmake
  DESTINATION ${CMAKE_MODULE_INSTALL_PATH} COMPONENT dev )