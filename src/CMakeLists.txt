cmake_minimum_required(VERSION 3.24)

project(quanserNetwork C CXX)
include(FetchContent)

###################
# Mablab location #
###################
set(Matlab_ROOT_DIR /usr/local/MATLAB/R2023a)

####################
# Quanser location #
####################
if (MSVC)
    # Windows systems
    # todo make cmake test if quarc or hil sdk is installed
    set(QUANSER_PATH "C:/Program Files/Quanser/HIL SDK")
	if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		message(STATUS "(INFO) Detected Windows 32 bits")
		set(QUANSER_LIB_PATH ${QUANSER_PATH}/lib/windows)
	else ()
		message(STATUS "(INFO) Detected Windows 64 bits")
		set(QUANSER_LIB_PATH ${QUANSER_PATH}/lib/win64)
	endif ()
	set(QUANSER_INCLUDE_PATH ${QUANSER_PATH}/include)
else ()
    # Linux systems (only HIL SDK is supported)
    set(QUANSER_PATH /opt/quanser/hil_sdk)
    set(QUANSER_LIB_PATH ${QUANSER_PATH}/lib)
    set(QUANSER_INCLUDE_PATH ${QUANSER_PATH}/include)
endif ()
if (EXISTS ${QUANSER_PATH})
    set (BUILD_SERVER ON)
    message (STATUS "The server application will be built")
else ()
    set (BUILD_SERVER OFF)
    message (STATUS "The server application will NOT be built")
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

set(SRC_UTILS
    utils/definitions.hpp
    utils/definitions.cpp
    utils/protocol.c
    utils/protocol.h
    utils/receiver.c
    utils/receiver.h
    utils/time_helper.cpp
    utils/time_helper.h
    utils/memory_table.cpp
    utils/memory_table.hpp
)

################################
# Build the server application #
################################
if (${BUILD_SERVER} MATCHES ON)
    message (STATUS "Configuring to build the server application")
    add_executable(server
        server/main.cpp
        server/server.cpp
        server/server.hpp
        ${SRC_UTILS}
    )
    if (MSVC)
        set(QUANSER_REQUIRED_LIBS hil quanser_runtime quanser_common ntdll)
    else ()
        set(QUANSER_REQUIRED_LIBS hil quanser_runtime quanser_common rt pthread dl m c)
    endif ()
    target_link_libraries(server ${QUANSER_REQUIRED_LIBS})
    target_link_directories(server PRIVATE ${QUANSER_LIB_PATH})
    target_include_directories(server PRIVATE "." ${ASIO_INCLUDE_PATH} ${QUANSER_INCLUDE_PATH})
endif ()

##############################
# Build the matlab sfunction #
##############################
if (EXISTS ${Matlab_ROOT_DIR})
    find_package(Matlab)
    if (BUILD_SFUNCTION MATCHES ON)
        matlab_add_mex(NAME sfunction SRC
            client/remote_board_sfunction.cpp
            client/remote_board.cpp
            client/remote_board.hpp
            ${SRC_UTILS}
        )
        target_include_directories(sfunction PRIVATE
            "."
            ${ASIO_INCLUDE_PATH}
            "${Matlab_ROOT_DIR}/simulink/include"
        )
        target_compile_definitions(sfunction PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    endif ()
endif ()

#####################################
# Build the analog_loopback example #
#####################################
add_executable(exampleAnalogLoopback
    examples/analog_loopback.cpp
    client/remote_board.cpp
    client/remote_board.hpp
    ${SRC_UTILS}
)
target_include_directories(exampleAnalogLoopback PRIVATE "." ${ASIO_INCLUDE_PATH})
if (UNIX)
    target_link_libraries(exampleAnalogLoopback pthread)
endif ()

#####################################
# Build the autoread example #
#####################################
add_executable(exampleAutoRead
    examples/auto_read.cpp
    client/remote_board.cpp
    client/remote_board.hpp
    ${SRC_UTILS}
)
target_include_directories(exampleAutoRead PRIVATE "." ${ASIO_INCLUDE_PATH})
if (UNIX)
    target_link_libraries(exampleAutoRead pthread)
endif ()

###############################################
# Build the memory table verifier application #
###############################################
add_executable(scriptTestMemoryTable
    scripts/test_memory_table.cpp
    ${SRC_UTILS}
)
target_include_directories(scriptTestMemoryTable PRIVATE ".")

# todo copy the Quanser dlls automatically
# https://stackoverflow.com/questions/10671916/how-to-copy-dll-files-into-the-same-folder-as-the-executable-using-cmake

###############
# C++ options #
###############
set(CMAKE_CXX_STANDARD 11)
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
