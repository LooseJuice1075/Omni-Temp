#pragma once

#include <memory>

#ifdef OM_PLATFORM_WINDOWS
#if OM_DYNAMIC_LINK
	#ifdef OM_BUILD_DLL
		#define OMNI_API __declspec(dllexport)
	#else
		#define OMNI_API __declspec(dllimport)
	#endif
#else
	#define OMNI_API
#endif
#else
	#error Omni only supports windows!
#endif

#ifdef OM_DEBUG
	#define OM_ENABLE_ASSERTS
#endif

#ifdef OM_ENABLE_ASSERTS
	#define OM_ASSERT(x, ...) { if(!(x))} { OM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define OM_CORE_ASSERT(x, ...) { if(!(x)) { OM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define OM_ASSERT(x, ...)
	#define OM_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define OM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Omni
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
