#include "OpenAILibLLM.h"
#include "Instructions.h"
#include "liboai.h"

namespace LibBlueprintCopilot::Guidance
{
    class OpenAILibLLM::Impl
    {
    public:
        Impl(std::string_view api_key)
        {
            if (!oai.auth.SetKey(api_key))
            {
                // Handle problem with auth
            }

            oai.auth.SetMaxTimeout(3 * 1000 * 60);
        }
        
        // TODO pass in the model as well
        std::string Request(std::string_view input, std::string_view model)
        {
            try
            {
                convo.AddUserData(input);
                auto response = oai.ChatCompletion->create(std::string{model}, convo);

                convo.Update(response);

                return convo.GetLastResponse();
            }
            catch (std::exception& e)
            {
                std::cout << e.what() << std::endl;
                return e.what();
            }
        }

    public:
        liboai::OpenAI oai;
        liboai::Conversation convo{SystemData};
    };

    OpenAILibLLM::OpenAILibLLM(std::string_view api_key) : _api_key{api_key}, _pimpl{std::make_unique<Impl>(api_key)} {}
    OpenAILibLLM::OpenAILibLLM(OpenAILibLLM&& other) noexcept = default;
    OpenAILibLLM& OpenAILibLLM::operator=(OpenAILibLLM&& other) noexcept = default;
    OpenAILibLLM::~OpenAILibLLM() = default;

    std::string OpenAILibLLM::Request(std::string_view input, std::string_view model)
    {
        return _pimpl->Request(input, model);
    }
} // namespace LibBlueprintCopilot::Guidance