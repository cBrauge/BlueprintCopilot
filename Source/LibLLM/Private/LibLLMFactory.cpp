namespace LibLLM {
	ILibLLM LibLLMFactory::CreateLibLLM(LibLLMModel model, std::string_view api_key)
	{
		if (model == LibLLMModel::FakeLLMModel)
		{
			return FakeLibLLM();
		}
		else if (model == LibLLMModel::OpenAILLMModel)
		{
			return OpenAILibLLM(api_key);
		}
	}
}