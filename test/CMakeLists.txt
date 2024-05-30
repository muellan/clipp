project(clipp-tests)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} source_files)

if (BUILD_TESTING_SINGLE)
  add_executable(${PROJECT_NAME})
  target_link_libraries(${PROJECT_NAME} clipp::clipp)
  target_compile_features(${PROJECT_NAME} PUBLIC ${CLIPP_CXX_STD})
  target_compile_definitions(${PROJECT_NAME} PRIVATE
      ALL_TESTS_AS_SINGLE_PRJ
      _XTRA_FUNCTIONAL
  )
  foreach(src IN LISTS source_files)
    get_filename_component(name_we ${src} NAME_WE)
    target_sources(${PROJECT_NAME} PRIVATE ${src})
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-DTHIS_TEST_FNAME=${name_we}")
  endforeach()
  target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/pch.h
    ${CMAKE_CURRENT_SOURCE_DIR}/testing.h
    ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.py
    ${CMAKE_SOURCE_DIR}/include/clipp.h
    ${CMAKE_SOURCE_DIR}/include/clipp-narrow.h
    ${CMAKE_SOURCE_DIR}/include/clipp-wide.h
    ${CMAKE_SOURCE_DIR}/ALL_test.cpp
  )
  target_precompile_headers(${PROJECT_NAME} PRIVATE
      "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
  )
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_files})
  source_group("CLIPP" FILES
    ${CMAKE_SOURCE_DIR}/include/clipp.h
    ${CMAKE_SOURCE_DIR}/include/clipp-narrow.h
    ${CMAKE_SOURCE_DIR}/include/clipp-wide.h
  )
  set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER "tests"
  )
  if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
  endif()
else()
  foreach(src IN LISTS source_files)
    get_filename_component(name_we ${src} NAME_WE)
    add_executable(test-${name_we} ${src})
    target_compile_features(test-${name_we} PUBLIC ${CLIPP_CXX_STD})
    add_test(NAME ${name_we} COMMAND $<TARGET_FILE:test-${name_we}>)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src})
    target_link_libraries(test-${name_we} clipp::clipp)
    set_target_properties(test-${name_we} PROPERTIES
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
      FOLDER "tests"
    )
  endforeach()
endif()
