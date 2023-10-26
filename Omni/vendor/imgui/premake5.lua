project "ImGui"
	kind "StaticLib"
	language "C++"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    defines
    {
        "IMGUI_API=__declspec(dllexport)"
    }

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_tables.cpp",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp",
        "backends/imgui_impl_dx9.h",
        "backends/imgui_impl_dx9.cpp",
        "backends/imgui_impl_dx10.h",
        "backends/imgui_impl_dx10.cpp",
        "backends/imgui_impl_dx11.h",
        "backends/imgui_impl_dx11.cpp",
        "backends/imgui_impl_opengl2.cpp",
        "backends/imgui_impl_opengl2.h",
        "backends/imgui_impl_opengl3.cpp",
        "backends/imgui_impl_opengl3.h",
        "backends/imgui_impl_opengl3_loader.h",
        "backends/imgui_impl_win32.cpp",
        "backends/imgui_impl_win32.h",
        "misc/cpp/imgui_stdlib.cpp",
        "misc/cpp/imgui_stdlib.h",
        "misc/single_file/imgui_single_file.h"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "on"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
