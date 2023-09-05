#pragma once

#include <memory>
#include <string>

namespace LibBlueprintCopilot::Guidance {
	class ChatGPT {
	public:
		ChatGPT(std::string_view api_key);
		~ChatGPT();

		std::string MakeRequest(std::string&& request);
	private:
		std::string _api_key;
	private:
		class Impl;
		std::unique_ptr<Impl> _pimpl;
	};
}