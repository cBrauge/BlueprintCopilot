cmake_minimum_required(VERSION 3.26)

project(oai)
add_library(oai STATIC
    ../../liboai/liboai/components/audio.cpp
    ../../liboai/liboai/components/azure.cpp
    ../../liboai/liboai/components/chat.cpp
    ../../liboai/liboai/components/completions.cpp
    ../../liboai/liboai/components/edits.cpp
    ../../liboai/liboai/components/embeddings.cpp
    ../../liboai/liboai/components/files.cpp
    ../../liboai/liboai/components/fine_tunes.cpp
    ../../liboai/liboai/components/images.cpp
    ../../liboai/liboai/components/models.cpp
    ../../liboai/liboai/components/moderations.cpp
    ../../liboai/liboai/core/authorization.cpp
    ../../liboai/liboai/core/netimpl.cpp
    ../../liboai/liboai/core/response.cpp

    ../../liboai/liboai/include/components/audio.h
    ../../liboai/liboai/include/components/azure.h
    ../../liboai/liboai/include/components/chat.h
    ../../liboai/liboai/include/components/completions.h
    ../../liboai/liboai/include/components/edits.h
    ../../liboai/liboai/include/components/embeddings.h
    ../../liboai/liboai/include/components/files.h
    ../../liboai/liboai/include/components/fine_tunes.h
    ../../liboai/liboai/include/components/images.h
    ../../liboai/liboai/include/components/models.h
    ../../liboai/liboai/include/components/moderations.h
    ../../liboai/liboai/include/core/authorization.h
    ../../liboai/liboai/include/core/exception.h
    ../../liboai/liboai/include/core/netimpl.h
    ../../liboai/liboai/include/core/network.h
    ../../liboai/liboai/include/core/response.h)

target_link_libraries(oai
    PRIVATE nlohmann_json libcurl
)

target_include_directories(oai
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../liboai/liboai/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(oai PROPERTIES LINKER_LANGUAGE CXX)

# Create the Guidance library
project(LibGuidance)
if(USE_CHATGPT)
    set(IMPLEMENTATION_SOURCES src/ChatGPT.cpp)
else()
    set(IMPLEMENTATION_SOURCES src/FakeChatGPT.cpp)
endif()

add_library(LibGuidance STATIC
    include/LibBlueprintCopilot.h
    include/ChatGPT.h
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
    include/ActionsDeserialization.h
    src/OpenAILibLLM.cpp
    src/LibLLMFactory.cpp
    src/FakeLibLLM.cpp
    src/LibBlueprintCopilot.cpp
    src/ActionsDeserialization.cpp
    ${IMPLEMENTATION_SOURCES}
)

target_link_libraries(LibGuidance
    PRIVATE nlohmann_json libcurl oai
)

target_include_directories(LibGuidance
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(LibGuidance PROPERTIES LINKER_LANGUAGE CXX)
