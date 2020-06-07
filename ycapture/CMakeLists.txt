# Library
set( YCAPTURE_HEADER
    CaptureSource.h
    EventReceiver.h
    IReceiver.h
    IYCSource.h
    NullReceiver.h
    stdafx.h
    targetver.h
    ycapture.h
    YCCritSection.h
    YCMutex.h
    yctypes.h
)
set( YCAPTURE_SOURCE
    CaptureSource.cpp
    dllmain.cpp
    EventReceiver.cpp
    NullReceiver.cpp
    stdafx.cpp
    ycapture.cpp
)

set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
add_library( ycapture SHARED ${YCAPTURE_HEADER} ${YCAPTURE_SOURCE} )
add_definitions( -DUNICODE )
add_definitions( -D_UNICODE )
target_include_directories( ycapture PRIVATE "./" )
target_include_directories( ycapture PRIVATE "../baseclasses/" )
target_link_libraries( ycapture PRIVATE strmbase strmiids.lib winmm.lib )

# Install
set_target_properties( ycapture PROPERTIES PUBLIC_HEADER "${YCAPTURE_HEADER}" )
install(
    TARGETS ycapture
    EXPORT ycapture-export
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

install(
    EXPORT ycapture-export
    FILE ycapture-config.cmake
    DESTINATION share/cmake/ycapture/
    EXPORT_LINK_INTERFACE_LIBRARIES
) 
