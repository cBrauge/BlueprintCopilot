#include "ActionsDeserialization.h"
#include "Examples.h"
#include "LibLLMFactory.h"

#include <iostream>
#include <variant>

// DO NOT PUSH YOUR OWN API KEY
const auto ApiKey{""};
// Azure, DO NOT PUSH
const auto ResourceName{""};
const auto DeploymentId{""};

const auto Gpt3{"gpt-3.5-turbo"};
const auto Gpt4{"gpt-4"};

using namespace LibBlueprintCopilot::Guidance;

auto FakeExample()
{
    const auto libLLM{LibLLMFactory::CreateLibLLM(LibLLMModel::FakeLLMModel, ApiKey)};

    // Fake API just returns the input
    // const auto response{libLLM->Request(ListOfInstruccvtions, Gpt4)};
    const auto response{libLLM->Request(ListOfInstructionsNotNumber, Gpt4)};
    return ConvertResponseToActions(response);
}

auto RealExample()
{
    const auto libLLM{LibLLMFactory::CreateLibLLM(LibLLMModel::OpenAILLMModel, ApiKey)};

    // Real api, sending a real request
    const auto response{libLLM->Request(Request, Gpt4)};
    return ConvertResponseToActions(response);
}

auto RealAzureExample()
{
    const auto libLLM{LibLLMFactory::CreateLibLLM(LibLLMModel::AzureLLMModel, ApiKey, ResourceName, DeploymentId)};

    // Real api, sending a real request
    const auto response{libLLM->Request(Request, Gpt4)};
    return ConvertResponseToActions(response);
}

void PrintActions(const auto& actions)
{
    auto i{0};
    for (const auto& action : actions)
    {
        std::visit([&i](const auto& a) -> void { std::cout << i++ << " " << PrettyPrint(a) << std::endl; }, action);
    }
}

int main()
{
    // const auto actions{FakeExample()};
    //  const auto actions{RealExample()};
    const auto actions{RealAzureExample()};

    if (std::holds_alternative<ParseError>(actions))
    {
        std::cerr << std::get<ParseError>(actions).Reason << std::endl;
        return 1;
    }

    PrintActions(std::get<std::vector<Action>>(actions));
}