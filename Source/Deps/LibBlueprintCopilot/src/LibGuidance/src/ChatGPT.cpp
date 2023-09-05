// Real implementation that calls ChatGPT

#include "ChatGPT.h"

#include "liboai.h"

namespace LibBlueprintCopilot::Guidance
{
    class ChatGPT::Impl
    {
    public:
        liboai::OpenAI oai;
    };

    ChatGPT::ChatGPT(std::string_view api_key) : _api_key{api_key}, _pimpl{std::make_unique<Impl>()} {}

    ChatGPT::~ChatGPT() = default;

    std::string ChatGPT::MakeRequest(std::string&& input)
    {
        return "Real";
    }
} // namespace LibBlueprintCopilot::Guidance