cmake_minimum_required(VERSION 3.8)
project(clipp
  VERSION 1.2.3
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(GNUInstallDirs)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CLIPP_CXX_STD cxx_std_17)

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE ${CLIPP_CXX_STD})
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(CONFIG_VERSION_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake)
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

option(BUILD_TESTING "Do not build tests by default" OFF)
option(BUILD_TESTING_SINGLE "Build all test as single project" ON)
option(BUILD_EXAMPLES "Do not build examples by default" OFF)
include(CTest)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  set(CLIPP_COMPILER_OPTS
    /W4
    /EHsc
  )
  set(COMPILER_IS_CL 1)
else()
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    #add_compile_options(-stdlib=libc++) # clang version 17.0.6 failed with this option
    # TODO: check clang version
    set(CLIPP_COMPILER_OPTS
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
    )
    set(COMPILER_IS_CLANG 1)
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "gcc")
    set(CLIPP_COMPILER_OPTS
      -Wlogical-op
      -Wnoexcept
      -Wstrict-null-sentinel
      -Wuseless-cast
    )
    set(COMPILER_IS_GCC 1)
  endif()
  #if (COMPILER_IS_CLANG OR COMPILER_IS_GCC)
    list(APPEND CLIPP_COMPILER_OPTS
      -Wall
      -Wextra
      -Wpedantic
      -Wcast-align
      -Wcast-qual
      -Wctor-dtor-privacy
      -Wconversion -Wno-sign-conversion
      -Wdisabled-optimization
      -Wdouble-promotion
      -Wformat=2
      -Winit-self
      -Wmissing-include-dirs
      -Wold-style-cast
      -Woverloaded-virtual
      -Wredundant-decls
      -Wshadow
      -Wstrict-aliasing=1
      -Wstrict-overflow=5
      -Wswitch-default
      -Wundef
    )
  #endif()
endif()

add_compile_options(${CLIPP_COMPILER_OPTS})

message(STATUS "CMAKE_BUILD_TYPE      : ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER_ID : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CLIPP_CXX_STD         : ${CLIPP_CXX_STD}")
message(STATUS "CLIPP_COMPILER_OPTS   : ${CLIPP_COMPILER_OPTS}")

message(STATUS "COMPILER_IS_CL        : ${COMPILER_IS_CL}")
message(STATUS "COMPILER_IS_CLANG     : ${COMPILER_IS_CLANG}")
message(STATUS "COMPILER_IS_GCC       : ${COMPILER_IS_GCC}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(
        -D_DEBUG
    )
endif()
if (WIN32)
    add_definitions(
        -D_WIN32
        -D_AMD64_
    )
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

set(ROOT_FILES
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_SOURCE_DIR}/mk-msvs-solution.cmd
    ${CMAKE_SOURCE_DIR}/mk-msvs-solution-clang.cmd
    ${CMAKE_SOURCE_DIR}/mk-vars.cmd
    ${CMAKE_SOURCE_DIR}/mk-trace.cmd
)

target_sources(${PROJECT_NAME} PRIVATE ${ROOT_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "root")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ROOT_FILES})
