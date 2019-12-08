class LLWeb {
	public:
		static void InitClass();
		/// Load the given url in the user’s preferred web browser
		static void LoadURL(const std::string& url);
		/// Load the given url in the Second Life internal web browser 
		static void LoadURLInternal(const std::string &url);
		/// Load the given url in the operating system’s web browser 
		static void LoadURLExternal(const std::string& url);
		/// Returns escaped url (eg, " " to "%20")
		static std::string EscapeURL(const std::string& url); 
};
