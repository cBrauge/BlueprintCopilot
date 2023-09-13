cmake_minimum_required(VERSION 3.26)

project(LibGuidance)

add_library(LibGuidance STATIC
    include/AzureLibLLM.h
    include/FakeLibLLM.h
    include/ILibLLM.h
    include/LibLLMFactory.h
    include/LibLLMModel.h
    include/OpenAILibLLM.h
    include/Actions/Action.h
    include/Actions/BaseAction.h
    include/Actions/CreateBlueprint.h
    include/Actions/CreateBlueprintPermanently.h
    include/Actions/AddFunctionNode.h
    include/Actions/AddVariable.h
    include/Actions/AddVariableGetNode.h
    include/Actions/AddVariableSetNode.h
    include/Actions/CreateLink.h
    include/Actions/PositionNode.h
    include/Actions/UpdateBlueprint.h
    include/Actions/ManualOperation.h
    include/Actions/AddTextBlockToWidgetBlueprint.h
    include/Actions/AssignNode.h
    include/Actions/SetPinValue.h
    include/ActionsDeserialization.h
    include/ParseError.h
    include/Instructions.h
    include/Examples.h
    include/ParseError.h
    include/Azure.h
    src/OpenAILibLLM.cpp
    src/AzureLibLLM.cpp
    src/LibLLMFactory.cpp
    src/FakeLibLLM.cpp
    src/ActionsDeserialization.cpp
    src/Azure.cpp
 )

target_link_libraries(LibGuidance
    PRIVATE nlohmann_json libcurl oai
)
message(STATUS "oai_SOURCE_DIR: ${oai_SOURCE_DIR}")
message(STATUS "oai_BINARY_DIR: ${oai_BINARY_DIR}")

target_include_directories(LibGuidance
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${oai_SOURCE_DIR}/src/include
        ${CURL_INCLUDE_DIRS}
)

set_target_properties(LibGuidance PROPERTIES LINKER_LANGUAGE CXX)
