# Library
set( BASE_CLASSES_HEADER
    amextra.h
    amfilter.h
    cache.h
    combase.h
    cprop.h
    ctlutil.h
    ddmm.h
    dllsetup.h
    dxmperf.h
    fourcc.h
    measure.h
    msgthrd.h
    mtype.h
    outputq.h
    perflog.h
    perfstruct.h
    pstream.h
    pullpin.h
    refclock.h
    reftime.h
    renbase.h
    schedule.h
    seekpt.h
    source.h
    streams.h
    strmctl.h
    sysclock.h
    transfrm.h
    transip.h
    videoctl.h
    vtrans.h
    winctrl.h
    winutil.h
    wxdebug.h
    wxlist.h
    wxutil.h
)
set( BASE_CLASSES_SOURCE
    amextra.cpp
    amfilter.cpp
    amvideo.cpp
    arithutil.cpp
    combase.cpp
    cprop.cpp
    ctlutil.cpp
    ddmm.cpp
    dllentry.cpp
    dllsetup.cpp
    mtype.cpp
    outputq.cpp
    perflog.cpp
    pstream.cpp
    pullpin.cpp
    refclock.cpp
    renbase.cpp
    schedule.cpp
    seekpt.cpp
    source.cpp
    strmctl.cpp
    sysclock.cpp
    transfrm.cpp
    transip.cpp
    videoctl.cpp
    vtrans.cpp
    winctrl.cpp
    winutil.cpp
    wxdebug.cpp
    wxlist.cpp
    wxutil.cpp
)

add_library( strmbase STATIC ${BASE_CLASSES_HEADER} ${BASE_CLASSES_SOURCE} )
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
include_directories( "./" )
add_definitions( -DUNICODE )
add_definitions( -D_UNICODE )
add_definitions( -D_LIB )
target_link_libraries( strmbase strmiids.lib )

# Install
# set_target_properties( strmbase PROPERTIES PUBLIC_HEADER "${BASE_CLASSES_HEADER}" )
# install(
#     TARGETS strmbase
#     EXPORT strmbase-export
#     LIBRARY DESTINATION lib
#     PUBLIC_HEADER DESTINATION include
# )

# install(
#     EXPORT strmbase-export
#     FILE strmbase-config.cmake
#     DESTINATION share/cmake/strmbase/
#     EXPORT_LINK_INTERFACE_LIBRARIES
# ) 
