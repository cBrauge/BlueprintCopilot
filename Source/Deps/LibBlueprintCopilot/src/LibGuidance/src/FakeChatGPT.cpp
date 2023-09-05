// Fake GPT, doesn't make actual requests, just return string

#include "ChatGPT.h"

namespace LibBlueprintCopilot::Guidance
{
    class ChatGPT::Impl
    {
    };

    ChatGPT::ChatGPT(std::string_view api_key) : _api_key{api_key}, _pimpl{std::make_unique<Impl>()} {}

    ChatGPT::~ChatGPT() = default;

    std::string ChatGPT::MakeRequest(std::string&& input)
    {
        return "Fake";
    }
} // namespace LibBlueprintCopilot::Guidance