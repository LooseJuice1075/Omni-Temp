#pragma once

#include <memory>

#include "core/Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Omni
{
	class OMNI_API Log
	{
	public:
		static void Init(std::string configFilepath = "assets/log_settings.txt");

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		inline static bool s_LogInitMessage;
		inline static bool s_LogWindowEvents;
		inline static bool s_LogRendererInfo;
	private:
		static void ReadLoggerSettings(std::string filePath);
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros

#define OM_CORE_TRACE(...) ::Omni::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OM_CORE_INFO(...)  ::Omni::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OM_CORE_WARN(...)  ::Omni::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OM_CORE_ERROR(...) ::Omni::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OM_CORE_CRITICAL(...) ::Omni::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros

#define OM_TRACE(...) ::Omni::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OM_INFO(...)  ::Omni::Log::GetClientLogger()->info(__VA_ARGS__)
#define OM_WARN(...)  ::Omni::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OM_ERROR(...) ::Omni::Log::GetClientLogger()->error(__VA_ARGS__)
#define OM_CRITICAL(...) ::Omni::Log::GetClientLogger()->critical(__VA_ARGS__)