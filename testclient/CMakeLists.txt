set( TESTCLIENT_HEADER
    stdafx.h
    targetver.h
)
set( TESTCLIENT_SOURCE
    stdafx.cpp
    testclient.cpp
)

add_executable( testclient ${TESTCLIENT_HEADER} ${TESTCLIENT_SOURCE})
target_include_directories( testclient PRIVATE "./" )
target_include_directories( testclient PRIVATE "../ycapture/" )
target_include_directories( testclient PRIVATE "../ycaptureclient/" )
target_link_libraries( testclient ycapture ycaptureclient )

# Install
install(
    TARGETS testclient
    RUNTIME DESTINATION bin
)