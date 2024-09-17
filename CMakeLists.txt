cmake_minimum_required(VERSION 3.7)

if(${CMAKE_VERSION} VERSION_LESS 3.29)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.29)
endif()

project(BoseAudio VERSION 1.0
                  DESCRIPTION "Bose Audio module"
                  LANGUAGES C CXX)

# MS compiler and linker options
# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-170
# https://learn.microsoft.com/en-us/cpp/build/reference/linker-options?view=msvc-170#linker-options-listed-alphabetically

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/Include)
set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/Source)
set(BUILD_DLL_DIR ${CMAKE_CURRENT_LIST_DIR}/BuildDll)

# FrameDll.lib: Set path to FrameDll.lib in AWE Workspace
set(FRAME_DLL_LIB "C:/DSP Concepts/AWE Designer 8.D.2.6 Pro/Lib/win32-vc142-rel/FrameDll.lib")

# MSBuild flags and configurations
IF (MSVC)
    # Use static MFC library, compiles with -MD to use multi-threaded 
    # dynamically-linked runtime library
    add_definitions(-D_AFXDLL)
    set(CMAKE_MFC_FLAG 2)

    # Compile with -MT flags for multi-threaded statically-linked runtime library
    # set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
ENDIF (MSVC)

# BoseAudioLib.lib
add_library(BoseAudioLib STATIC 
            ${SOURCE_DIR}/ModTestAudio.c
            ${SOURCE_DIR}/mathwrap.c
            ${SOURCE_DIR}/BAF_callbacks.c)

target_include_directories(BoseAudioLib PUBLIC ${INCLUDE_DIR})
target_include_directories(BoseAudioLib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/build/CART/BAF/include)
target_include_directories(BoseAudioLib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/build/CART/CART/include)
target_include_directories(BoseAudioLib PUBLIC ${INCLUDE_DIR}/Targets)

# BoseAudio.dll
add_library(BoseAudio SHARED 
            ${INCLUDE_DIR}/TestAudio.h
            ${CMAKE_CURRENT_LIST_DIR}/TestAudioSchema.cpp
            ${BUILD_DLL_DIR}/ModulelibDLL.cpp
            ${BUILD_DLL_DIR}/ModuleLibDLLAudio.def
            ${BUILD_DLL_DIR}/ModuleTableAudio.cpp
            ${BUILD_DLL_DIR}/Resource.h
            ${BUILD_DLL_DIR}/stdafx.cpp
            ${BUILD_DLL_DIR}/stdafx.h)

target_include_directories(BoseAudio PUBLIC 
                           ${BUILD_DLL_DIR}
                           ${INCLUDE_DIR}
                           ${CMAKE_CURRENT_LIST_DIR}
                           ${SOURCE_DIR}
                           ${INCLUDE_DIR}/Targets)

# Disable safe exception handlers, unsupported by BAF library
target_link_options(BoseAudio PUBLIC "/SAFESEH:NO")

# Link the custom module library and FrameDll.lib to DLL
target_link_libraries(BoseAudio PUBLIC BoseAudioLib
                                PUBLIC ${FRAME_DLL_LIB}
                                PUBLIC ${CMAKE_CURRENT_LIST_DIR}/build/CART/BAF/lib/Baf2.lib)

# BoseControlLib.lib
add_library(BoseControlLib STATIC 
            ${SOURCE_DIR}/ModControl.c)

target_include_directories(BoseControlLib PUBLIC ${INCLUDE_DIR})

# BoseControl.dll
add_library(BoseControl SHARED 
            ${INCLUDE_DIR}/Control.h
            ${CMAKE_CURRENT_LIST_DIR}/ControlSchema.cpp
            ${BUILD_DLL_DIR}/ModulelibDLL.cpp
            ${BUILD_DLL_DIR}/ModuleLibDLLControl.def
            ${BUILD_DLL_DIR}/ModuleTableControl.cpp
            ${BUILD_DLL_DIR}/Resource.h
            ${BUILD_DLL_DIR}/stdafx.cpp
            ${BUILD_DLL_DIR}/stdafx.h)

target_include_directories(BoseControl PUBLIC 
                           ${BUILD_DLL_DIR}
                           ${INCLUDE_DIR}
                           ${CMAKE_CURRENT_LIST_DIR}
                           ${SOURCE_DIR}
                           ${INCLUDE_DIR}/Targets)

# Link the custom module library and FrameDll.lib to DLL
target_link_libraries(BoseControl PUBLIC BoseControlLib
                                  PUBLIC ${FRAME_DLL_LIB})

