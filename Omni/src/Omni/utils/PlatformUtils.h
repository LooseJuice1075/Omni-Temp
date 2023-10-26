#pragma once

#include <string>

namespace Omni
{
	class FileDialogs
	{
	public:
		// Returns an empty string if cancelled
		static std::string OpenFile(const char* filter);
		// Returns an empty string if cancelled
		static std::string SaveFile(const char* filter);
	};
}
