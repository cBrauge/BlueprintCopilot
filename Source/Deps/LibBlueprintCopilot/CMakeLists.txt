cmake_minimum_required (VERSION 3.26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Blueprint")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source to this project's executable. Could NOT find LibSSH2 (missing: LIBSSH2_LIBRARY LIBSSH2_INCLUDE_DIR) 
include(FetchContent)

FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz OVERRIDE_FIND_PACKAGE)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  curl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG        curl-7_77_0
  OVERRIDE_FIND_PACKAGE 
)

#set(CMAKE_USE_OPENSSL ON)
set(CMAKE_USE_SCHANNEL ON)
set(CMAKE_USE_LIBSSH2 OFF)

FetchContent_MakeAvailable(curl)

FetchContent_Declare(oai URL https://github.com/D7EAD/liboai/releases/download/v3.2.1/liboai-v3.2.1.zip OVERRIDE_FIND_PACKAGE)
FetchContent_GetProperties(oai)
if(NOT oai_POPULATED)
  FetchContent_Populate(oai)
endif()
file(GLOB OAI_SOURCES ${oai_SOURCE_DIR}/src/*/*.cpp ${oai_SOURCE_DIR}/src/core/netimpl.cpp)
add_library(oai STATIC ${OAI_SOURCES})
target_include_directories(oai PUBLIC ${oai_SOURCE_DIR}/include)

target_link_libraries(oai PRIVATE nlohmann_json libcurl)

get_property(_project_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach(_target IN LISTS _project_targets)
    message(STATUS "Target name: ${_target}")
endforeach()
message(STATUS "End target")

message(STATUS "OAI_SOURCES content:")
foreach(SOURCE IN LISTS OAI_SOURCES)
    message(STATUS "${SOURCE}")
endforeach()

add_subdirectory(src)
