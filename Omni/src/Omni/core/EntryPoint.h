#pragma once
#include "Omni/core/Core.h"
#include "Logo.h"

#ifdef OM_PLATFORM_WINDOWS

// Disable windows console when in distribution mode
#ifdef OM_DIST
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

extern Omni::Application* Omni::CreateApplication();

// TODO : Test to make sure this will work on any computer
// Tell the drivers to use the GPU instead of integrated graphics
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


int main(int argc, char** argv)
{
	Omni::Log::Init();
	if (Omni::Log::s_LogInitMessage)
		OM_CORE_TRACE(initText);

	OM_PROFILE_BEGIN_SESSION("Startup", "OmniProfile-Startup.json");
	auto app = Omni::CreateApplication();
	OM_PROFILE_END_SESSION();

	OM_PROFILE_BEGIN_SESSION("Runtime", "OmniProfile-Runtime.json");
	app->Run();
	OM_PROFILE_END_SESSION();

	OM_PROFILE_BEGIN_SESSION("Shutdown", "OmniProfile-Shutdown.json");
	delete app;
	OM_PROFILE_END_SESSION();
}

#endif
