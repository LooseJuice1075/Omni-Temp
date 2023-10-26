workspace "Omni"
	architecture "x64"
	startproject "Omni-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Omni/vendor/glfw/include"
IncludeDir["GLAD"] = "Omni/vendor/glad/include"
IncludeDir["ImGui"] = "Omni/vendor/imgui"
IncludeDir["Box2D"] = "Omni/vendor/box2d/include"
IncludeDir["entt"] = "Omni/vendor/entt"
IncludeDir["yaml_cpp"] = "Omni/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Omni/vendor/ImGuizmo"

group "Dependencies"
		include "Omni/vendor/imgui"
		include "Omni/vendor/glfw"
		include "Omni/vendor/glad"
		include "Omni/vendor/box2d"
		include "Omni/vendor/yaml-cpp"
group ""

project "Omni"
	location "Omni"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ompch.h"
	pchsource "Omni/src/Omni/ompch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		
		"%{prj.name}/vendor/glm/**.hpp",
		
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE",
		"STB_IMAGE_STATIC",
		"STB_IMAGE_IMPLEMENTATION",
		"STB_TRUETYPE_IMPLEMENTATION",
		"STB_RECT_PACK_IMPLEMENTATION",
	}
	
	includedirs
	{
		"%{prj.name}",
		"%{prj.name}/src",
		"%{prj.name}/src/Omni",
		"%{prj.name}/src/core",
		"%{prj.name}/src/events",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glm",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
	}
	
	filter "configurations:Debug"
		libdirs
		{
			"%{prj.name}/vendor/glfw/bin/Debug-windows-x86_64/GLFW",
			"%{prj.name}/vendor/glad/bin/Debug-windows-x86_64/GLAD",
			"%{prj.name}/vendor/imgui/bin/Debug-windows-x86_64/ImGui",
			"%{prj.name}/vendor/box2d/build/bin/Debug",
			"%{prj.name}/vendor/yaml-cpp/bin/Debug-windows-x86_64/yaml-cpp"
		}
	
		links
		{
			"GLFW",
			"GLAD",
			"opengl32.lib",
			"ImGui",
			"box2d",
			"yaml-cpp",
		}

	filter "configurations:Release"
		libdirs
		{
			"%{prj.name}/vendor/glfw/bin/Debug-windows-x86_64/GLFW",
			"%{prj.name}/vendor/glad/bin/Debug-windows-x86_64/GLAD",
			"%{prj.name}/vendor/imgui/bin/Debug-windows-x86_64/ImGui",
			"%{prj.name}/vendor/box2d/build/bin/MinSizeRel",
			"%{prj.name}/vendor/yaml-cpp/bin/Release-windows-x86_64/yaml-cpp"
		}
	
		links
		{
			"GLFW",
			"GLAD",
			"opengl32.lib",
			"ImGui",
			"box2d",
			"yaml-cpp",
		}

	filter "configurations:Dist"
		libdirs
		{
			"%{prj.name}/vendor/glfw/bin/Debug-windows-x86_64/GLFW",
			"%{prj.name}/vendor/glad/bin/Debug-windows-x86_64/GLAD",
			"%{prj.name}/vendor/imgui/bin/Debug-windows-x86_64/ImGui",
			"%{prj.name}/vendor/box2d/build/bin/MinSizeRel",
			"%{prj.name}/vendor/yaml-cpp/bin/Release-windows-x86_64/yaml-cpp"
		}
	
		links
		{
			"GLFW",
			"GLAD",
			"opengl32.lib",
			"ImGui",
			"box2d",
			"yaml-cpp",
		}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"OM_PLATFORM_WINDOWS",
			"OM_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "OM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OM_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/**.hpp",
	}
		
	includedirs
	{
		"Omni/src",
		"Omni/src/Omni",
		"Omni/include",
		"%{prj.name}",
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"Omni/vendor",
		"Omni/vendor/spdlog/include",
		"Omni/vendor/glm",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.entt}",
	}

	links
	{
		"Omni"
	}

	filter "system:windows"
		
		systemversion "latest"

		defines
		{
			"OM_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"STB_IMAGE_STATIC",
			"STB_IMAGE_IMPLEMENTATION",
			"STB_TRUETYPE_IMPLEMENTATION",
			"STB_TEXTEDIT_IMPLEMENTATION",
			"STB_RECT_PACK_IMPLEMENTATION",
		}

	filter "configurations:Debug"
		defines
		{
			"OM_DEBUG",
		}
		runtime "Debug"
		symbols "on"
		debugdir ("bin/" .. outputdir .. "/%{prj.name}")

	filter "configurations:Release"
		defines
		{
			"OM_RELEASE",
		}
		runtime "Release"
		optimize "on"
		debugdir ("bin/" .. outputdir .. "/%{prj.name}")

	filter "configurations:Dist"
		defines
		{
			"OM_DIST",
		}
		runtime "Release"
		optimize "on"
		debugdir ("bin/" .. outputdir .. "/%{prj.name}")

project "Omni-Editor"
	location "Omni-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/**.hpp",
	}
		
	includedirs
	{
		"Omni/src",
		"Omni/src/Omni",
		"Omni/include",
		"%{prj.name}",
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"Omni/vendor",
		"Omni/vendor/spdlog/include",
		"Omni/vendor/glm",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
	}

	links
	{
		"Omni"
	}

	filter "system:windows"
		
		systemversion "latest"

		defines
		{
			"OM_PLATFORM_WINDOWS",
			"YAML_CPP_STATIC_DEFINE",
			"STB_IMAGE_STATIC",
			"STB_IMAGE_IMPLEMENTATION",
			"STB_TRUETYPE_IMPLEMENTATION",
			"STB_RECT_PACK_IMPLEMENTATION",
		}

	filter "configurations:Debug"
		defines
		{
			"OM_DEBUG",
		}
		runtime "Debug"
		symbols "on"
		debugdir ("%{prj.name}")

	filter "configurations:Release"
		defines
		{
			"OM_RELEASE",
		}
		runtime "Release"
		optimize "on"
		debugdir ("%{prj.name}")

	filter "configurations:Dist"
		defines
		{
			"OM_DIST",
		}
		runtime "Release"
		optimize "on"
		debugdir ("%{prj.name}")