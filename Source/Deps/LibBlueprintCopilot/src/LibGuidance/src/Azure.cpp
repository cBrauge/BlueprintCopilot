#include "Azure.h"

using namespace liboai;


liboai::Response Myliboai::Azure::create_chat_completion(const std::string& resource_name,
    const std::string& deployment_id, const std::string& api_version, const Conversation& conversation,
    std::optional<float> temperature, std::optional<uint16_t> n,
    std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<std::vector<std::string>> stop,
    std::optional<uint16_t> max_tokens, std::optional<float> presence_penalty, std::optional<float> frequency_penalty,
    std::optional<std::unordered_map<std::string, int8_t>> logit_bias,
    std::optional<std::string> user) const& noexcept(false)
{
    liboai::JsonConstructor jcon;
    jcon.push_back("temperature", std::move(temperature));
    jcon.push_back("n", std::move(n));
    jcon.push_back("stream", stream);
    jcon.push_back("stop", std::move(stop));
    jcon.push_back("max_tokens", std::move(max_tokens));
    jcon.push_back("presence_penalty", std::move(presence_penalty));
    jcon.push_back("frequency_penalty", std::move(frequency_penalty));
    jcon.push_back("logit_bias", std::move(logit_bias));
    jcon.push_back("user", std::move(user));

    if (conversation.GetJSON().contains("messages"))
    {
        jcon.push_back("messages", conversation.GetJSON()["messages"]);
    }

    netimpl::components::Parameters params;
    params.Add({"api-version", api_version});

    Response res;
    res = this->Request(Method::HTTP_POST, ("https://" + resource_name + "/openai/deployments/" + deployment_id),
        "/chat/completions", "application/json", this->auth_.GetAzureAuthorizationHeaders(),
        netimpl::components::Body{jcon.dump()}, std::move(params),
        stream ? netimpl::components::WriteCallback{std::move(stream.value())} : netimpl::components::WriteCallback{},
        this->auth_.GetProxies(), this->auth_.GetProxyAuth(), this->auth_.GetMaxTimeout());

    return res;
}
