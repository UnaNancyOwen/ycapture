cmake_minimum_required( VERSION 3.6 )

# Language
enable_language( CXX )

# Compiler Settings
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Set Install Directory
if( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set( CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/ycapture" CACHE PATH "Please specify install directory." FORCE )
endif()

# Project
project( ycapture VERSION "0.1.1" LANGUAGES CXX )
add_subdirectory( baseclasses )
add_subdirectory( ycapture )
add_subdirectory( ycaptureclient )
add_subdirectory( testclient )
