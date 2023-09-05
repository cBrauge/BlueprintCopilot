namespace LibLLM {
	std::string FakeLibLLM::Request(std::string_view request)
	{
		return request;
	}
}