#include "AzureLibLLM.h"

#include "Instructions.h"
#include "liboai.h"

namespace LibBlueprintCopilot::Guidance
{
    class AzureLibLLM::Impl
    {
    public:
        Impl(std::string_view api_key, std::optional<std::string_view> azure_resource_name,
            std::optional<std::string_view> azure_deployment_id, std::optional<std::string_view> azure_api_version)
            : azureResourceName{azure_resource_name.value_or("")}, azureDeploymentId{azure_deployment_id.value_or("")},
              azureAPIVersion{azure_api_version.value_or("")}
        {
            if (!oai.auth.SetAzureKey(api_key))
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
                auto response =
                    oai.Azure->create_chat_completion(azureResourceName, azureDeploymentId, azureAPIVersion, convo);

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
        std::string azureResourceName;
        std::string azureDeploymentId;
        std::string azureAPIVersion;
    };

    AzureLibLLM::AzureLibLLM(std::string_view api_key, std::optional<std::string_view> azure_resource_name,
        std::optional<std::string_view> azure_deployment_id, std::optional<std::string_view> azure_api_version)
        : _api_key{api_key},
          _pimpl{std::make_unique<Impl>(api_key, azure_resource_name, azure_deployment_id, azure_api_version)}
    {
    }
    AzureLibLLM::AzureLibLLM(AzureLibLLM&& other) noexcept            = default;
    AzureLibLLM& AzureLibLLM::operator=(AzureLibLLM&& other) noexcept = default;
    AzureLibLLM::~AzureLibLLM()                                       = default;

    std::string AzureLibLLM::Request(std::string_view input, std::string_view model)
    {
        return _pimpl->Request(input, model);
    }
} // namespace LibBlueprintCopilot::Guidance