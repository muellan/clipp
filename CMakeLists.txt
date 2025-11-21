# =============================================================================
# A CMake build script for the `C++ Playground' project
# =============================================================================

# -----------------------------------------------------------------------------
# Define the minimum required version of CMake
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.8...3.29)


# -----------------------------------------------------------------------------
# Enable Hot Reload for MSVC compilers if supported.
# -----------------------------------------------------------------------------

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(
    CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
    )
endif()


# -----------------------------------------------------------------------------
# Define the project name and version
# -----------------------------------------------------------------------------

project(clipp
  VERSION 1.2.3
  DESCRIPTION "Command line interfaces for modern C++"
  LANGUAGES CXX
  )


# -----------------------------------------------------------------------------
# Define customizable user options
# -----------------------------------------------------------------------------

# option (BUILD_SHARED_LIBS "Build using shared libraries" ON)
option (BUILD_TESTS "Build with tests" OFF)


# -----------------------------------------------------------------------------
# Configure build options
# -----------------------------------------------------------------------------

include(GNUInstallDirs)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Show which project we are building
message(STATUS
  "Configuring build for `"
  ${PROJECT_NAME}
  "' v"
  ${PROJECT_VERSION}
  " ..."
  )

# Set the build type to MinSizeRel if not specified
if (NOT DEFINED CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE MinSizeRel)
endif ()
message(STATUS "Build type set to: `" ${CMAKE_BUILD_TYPE} "' ...")

# If build type is Debug set for debugging code to compile
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Use debugging code set to: `ON' ...")
  set(USE_DEBUG ON)
else ()
  message(STATUS "Use debugging code set to: `OFF' ...")
  set(USE_DEBUG OFF)
endif ()

# Set custom compiler options
message (STATUS "Setting custom compiler options ...")
include ("${CMAKE_SOURCE_DIR}/CMake/set_compiler_flags.cmake")
set_compiler_flags()
message (STATUS "Custom compiler options set to: `"
  ${CMAKE_CXX_FLAGS} " "
  ${CMAKE_CXX_FLAGS_DEBUG}
  "' ..."
  )

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
target_include_directories(
  ${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

set(
  CONFIG_VERSION_FILE
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  )
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CONFIG_VERSION_FILE} COMPATIBILITY AnyNewerVersion
  )

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h"
  )
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-config
  )
install(EXPORT ${PROJECT_NAME}-config
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  )
install(FILES ${CONFIG_VERSION_FILE}
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}
  )


# -----------------------------------------------------------------------------
# Check if we are building with the unit tests
# -----------------------------------------------------------------------------

# Check if the tests are enabled
if (NOT DEFINED BUILD_TESTS)
  set (BUILD_TESTS OFF)  # Default to OFF
endif ()
message(STATUS "Build tests set to: `" ${BUILD_TESTS} "' ...")

# Add the test files directory if the tests are enabled
if(BUILD_TESTS AND ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
  include(CTest)
  add_subdirectory(test)
endif()


# End of `CMakeLists.txt'
