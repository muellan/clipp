project(clipp-examples)
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC ${CLIPP_CXX_STD})
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ALL_EXAMPLES_AS_SINGLE_PRJ
)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} source_files)
foreach(src IN LISTS source_files)
  get_filename_component(name_we ${src} NAME_WE)
  target_sources(${PROJECT_NAME} PRIVATE ${src})
  set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-DTHIS_EXAMPLE_FNAME=${name_we}")
endforeach()
target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
)
target_sources(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/example-def.h
  ${CMAKE_CURRENT_SOURCE_DIR}/pch.h
  ${CMAKE_SOURCE_DIR}/include/clipp.h
  ${CMAKE_SOURCE_DIR}/include/clipp-narrow.h
  ${CMAKE_SOURCE_DIR}/include/clipp-wide.h
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
  FOLDER "examples"
)
