cmake_minimum_required(VERSION 3.24)

project(quanserNetwork C CXX)
include(FetchContent)

#################
# What to build #
#################
set (BUILD_SERVER ON)           # it requires the Quanser HIL SDK (or QuaRC) to be installed
set (BUILD_CLIENT_EXAMPLES ON)

####################
# Quanser location #
####################
if (MSVC)
    set(QUANSER_PATH "C:/Program Files/Quanser/HIL SDK")
else ()
    set(QUANSER_PATH /opt/quanser/hil_sdk)
endif ()
set(QUANSER_INCLUDE_PATH ${QUANSER_PATH}/include)
if (MSVC)
	if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		message(STATUS "(INFO) Detected Windows 32 bits")
		set(QUANSER_LIB_PATH ${QUANSER_PATH}/lib/windows)
	else ()
		message(STATUS "(INFO) Detected Windows 64 bits")
		set(QUANSER_LIB_PATH ${QUANSER_PATH}/lib/win64)
	endif ()
else ()
    set(QUANSER_LIB_PATH ${QUANSER_PATH}/lib)
endif ()

###################
# Asio dependency #
###################
message(STATUS "Downloading the asio library")
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        c465349fa5cd91a64bb369f5131ceacab2c0c1c3 # v1.28.0
)
FetchContent_MakeAvailable(asio)
set(ASIO_INCLUDE_PATH ${asio_SOURCE_DIR}/asio/include)

###########################
# Build the utils library #
###########################
add_library(libUtils STATIC utils/definitions.hpp utils/definitions.cpp utils/protocol.c utils/protocol.h utils/receiver.c utils/receiver.h utils/time_helper.cpp utils/time_helper.h utils/memory_table.cpp utils/memory_table.hpp)
target_include_directories(libUtils PRIVATE ./utils)

################################
# Build the server application #
################################
if (${BUILD_SERVER} MATCHES ON)
    message (STATUS "Configuring to build the server application")
    add_executable(server server/main.cpp server/server.cpp)
    if (MSVC)
        set(QUANSER_REQUIRED_LIBS hil quanser_runtime quanser_common)
    else ()
        set(QUANSER_REQUIRED_LIBS hil quanser_runtime quanser_common rt pthread dl m c)
    endif ()
    target_link_libraries(server libUtils ${QUANSER_REQUIRED_LIBS})
    target_link_directories(server PRIVATE ${QUANSER_LIB_PATH})
    target_include_directories(server PRIVATE "." ${ASIO_INCLUDE_PATH} ${QUANSER_INCLUDE_PATH})
endif ()

#################################
# Build the RemoteBoard library #
#################################
add_library(libClient STATIC client/remote_board.cpp utils/memory_table.cpp)
target_link_libraries(libClient libUtils)
target_include_directories(libClient PRIVATE "." ${ASIO_INCLUDE_PATH})

#################################
# Build the RemoteBoard example #
#################################
if (${BUILD_CLIENT_EXAMPLES} MATCHES ON)
    add_executable(analogLoopbackExample examples/analog_loopback.cpp)
    target_link_libraries(analogLoopbackExample libClient)
    target_include_directories(analogLoopbackExample PRIVATE "." ${ASIO_INCLUDE_PATH})
endif ()

###############################################
# Build the memory table verifier application #
###############################################
add_executable(scriptTestMemoryTable scripts/test_memory_table.cpp)
target_include_directories(scriptTestMemoryTable PRIVATE ".")
target_link_libraries(scriptTestMemoryTable libUtils)

# todo copy the Quanser dlls automatically
# https://stackoverflow.com/questions/10671916/how-to-copy-dll-files-into-the-same-folder-as-the-executable-using-cmake

###############
# C++ options #
###############
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Warnings and optimization
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    if (MSVC)
        add_compile_options(/O2 /W3 /WX)
    else()
        add_compile_options(-Wall -Wextra -pedantic -Ofast)
    endif()
else ()
    message(STATUS "Minimal optimization, debug info included")
    if (MSVC)
        add_compile_options(/Od /W3 /WX)
    else()
        add_compile_options(-Wall -Wextra -pedantic -Og -g)
    endif()
endif ()
