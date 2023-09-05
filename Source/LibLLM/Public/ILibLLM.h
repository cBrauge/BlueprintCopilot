namespace LibLLM {
    class ILibLLM {
    public:
        virtual ~ILibLLM() {}

        /// <summary>
        /// Sends a request to the llm and responds with the response from the llm
        /// </summary>
        std::string Request(std::string_view request) = 0;
    };
}