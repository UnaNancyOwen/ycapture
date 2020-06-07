# Library
set( YCAPTURECLIENT_HEADER
    CaptureSender.h
    stdafx.h
    targetver.h
)
set( YCAPTURECLIENT_SOURCE
  CaptureSender.cpp
  stdafx.cpp
)

set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
add_library( ycaptureclient STATIC ${YCAPTURECLIENT_HEADER} ${YCAPTURECLIENT_SOURCE} )
include_directories( "./" )
include_directories( "../ycapture/" )
add_definitions( -DUNICODE )
add_definitions( -D_UNICODE )
add_definitions( -D_LIB )

# Install
set_target_properties( ycaptureclient PROPERTIES PUBLIC_HEADER "${YCAPTURECLIENT_HEADER}" )
install(
    TARGETS ycaptureclient
    EXPORT ycaptureclient-export
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

install(
    EXPORT ycaptureclient-export
    FILE ycaptureclient-config.cmake
    DESTINATION share/cmake/ycaptureclient/
    EXPORT_LINK_INTERFACE_LIBRARIES
) 
