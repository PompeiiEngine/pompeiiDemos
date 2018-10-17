include( glsl2spv )

glsl2spv( )

include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/routes.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/routes.h)


set( POMPEIIVKINFO_HEADERS )
set( POMPEIIVKINFO_SOURCES VkInfo.cpp )
set( POMPEIIVKINFO_LINK_LIBRARIES pompeii )
common_application( pompeiiVkInfo )


add_subdirectory( compute )
add_subdirectory( render )