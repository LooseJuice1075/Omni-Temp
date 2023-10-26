#include "ompch.h"

#include "Log.h"

namespace Omni
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init(std::string configFilepath)
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("[OMNI]");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("[APP]");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_LogInitMessage = false;
		s_LogWindowEvents = true;
		s_LogRendererInfo = true;

		ReadLoggerSettings(configFilepath);
	}

	void Log::ReadLoggerSettings(std::string filePath)
	{
		std::string fileName = "log_settings.txt";
		std::stringstream buffer;
		std::ifstream file(filePath);
		if (file.is_open()){
			buffer << file.rdbuf();
			file.close();

			std::string line;
			while (std::getline(buffer, line))
			{
				if (line == "LogInitMessage = FALSE")
					s_LogInitMessage = false;
				else if (line == "LogInitMessage = TRUE")
					s_LogInitMessage = true;

				if (line == "LogWindowEvents = FALSE")
					s_LogWindowEvents = false;
				else if (line == "LogWindowEvents = TRUE")
					s_LogWindowEvents = true;

				if (line == "LogRendererInfo = FALSE")
					s_LogRendererInfo = false;
				else if (line == "LogRendererInfo = TRUE")
					s_LogRendererInfo = true;
			}
		}
		else{
			std::cout << "[LOG]: Log settings file could not be opened, attempting to creating new file \"" << fileName << "\"." << std::endl;
			std::ofstream file(filePath);
			if (file.is_open()) {
				std::string fileContent = R"(LogInitMessage = FALSE
LogWindowEvents = TRUE
LogRendererInfo = TRUE)";
				file << fileContent;
				file.close();

				s_LogInitMessage = false;
				s_LogWindowEvents = true;
				s_LogRendererInfo = true;
			}
			else {
				std::cout << "[LOG]: File " << filePath << " could not be opened!" << std::endl;
			}
		}
	}
}
