cmake_minimum_required(VERSION 3.10)

project(TestApplication)

add_executable(TestApplication
    main.cpp
)

target_include_directories(TestApplication PRIVATE 
        ${oai_SOURCE_DIR}/src/include
)

target_link_libraries(TestApplication
    PRIVATE
        nlohmann_json libcurl oai
        LibGuidance
)

if (CMAKE_IMPORT_LIBRARY_SUFFIX)
  add_custom_command(
    TARGET TestApplication POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:TestApplication> $<TARGET_FILE_DIR:TestApplication>
    COMMAND_EXPAND_LISTS
  )
endif ()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestApplication PROPERTY CXX_STANDARD 20)
endif()