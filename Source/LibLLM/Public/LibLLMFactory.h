namespace LibLLM {
	class LibLLMFactory {
	public:
		static ILibLLM CreateLibLLM(LibLLMModel model, std::string_view api_key);
	};
}