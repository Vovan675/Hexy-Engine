workspace "Hexy"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}

	flags {
		"MultiProcessorCompile"
	}
	startproject "Editor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SpdLog"] = "Hexy/vendor/SpdLog/include"
IncludeDir["GLFW"] = "Hexy/vendor/GLFW/include"
IncludeDir["Glad"] = "Hexy/vendor/Glad/include"
IncludeDir["ImGui"] = "Hexy/vendor/imgui"
IncludeDir["ImGuizmo"] = "Hexy/vendor/imguizmo"
IncludeDir["ENTT"] = "Hexy/vendor/entt/include"
IncludeDir["GLM"] = "Hexy/vendor/glm"
IncludeDir["CEREAL"] = "Hexy/vendor/CEREAL/include"
IncludeDir["Assimp"] = "Hexy/vendor/Assimp/assimp/include"
IncludeDir["STB_IMAGE"] = "Hexy/vendor/stb"
IncludeDir["Mono"] = "Hexy/vendor/Mono/include"

LibDir = {}
LibDir["Mono"] = "vendor/Mono/lib"

group "Dependencies"

include "Hexy/vendor/GLFW"
include "Hexy/vendor/Glad"
include "Hexy/vendor/ImGui"
include "Hexy/vendor/ImGuizmo"
include "Hexy/vendor/Assimp"

group ""

project "ScriptCore"
	location "ScriptCore"
	kind "SharedLib"
	language "C#"

	targetdir ("Editor/assets/scripts/")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.cs",
	}

project "Hexy"
	location "Hexy"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Hexy/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.STB_IMAGE}/stb_image.h",
		"%{IncludeDir.STB_IMAGE}/stb_image.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{IncludeDir.SpdLog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.CEREAL}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.STB_IMAGE}",
		"%{IncludeDir.Mono}"
	}

	links { 
		"GLFW",
		"Glad",
		"ImGui",
		"ImGuizmo",
		"Assimp",
		"opengl32.lib",
		"%{LibDir.Mono}/mono-2.0-sgen.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"_CRT_SECURE_NO_WARNINGS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"


project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Hexy/src",
		"%{IncludeDir.SpdLog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.CEREAL}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.STB_IMAGE}",
		"%{IncludeDir.Mono}"
	}

	links {
		"Hexy"
	}

	filter "system:windows"
		systemversion "latest"
		files { '%{prj.name}/resources.rc', '%{prj.name}/**.ico' }

	filter "configurations:Debug"
		symbols "On"
		postbuildcommands {
			'{COPY} "../%{prj.name}/EditorResources" "%{cfg.targetdir}/EditorResources"',
			'{COPY} "../%{prj.name}/assets" "%{cfg.targetdir}/assets"',
			'{COPY} "../%{prj.name}/mono" "%{cfg.targetdir}"/mono',
			'{COPY} "../Hexy/vendor/Mono/bin/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Release"
		optimize "On"
		postbuildcommands {
			'{COPY} "../%{prj.name}/EditorResources" "%{cfg.targetdir}/EditorResources"',
			'{COPY} "../%{prj.name}/assets" "%{cfg.targetdir}/assets"',
			'{COPY} "../%{prj.name}/mono" "%{cfg.targetdir}"/mono',
			'{COPY} "../Hexy/vendor/Mono/bin/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
		}


project "GamePlayer"
	location "GamePlayer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Hexy/src",
		"%{IncludeDir.SpdLog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.CEREAL}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.STB_IMAGE}",
		"%{IncludeDir.Mono}"
	}

	links {
		"Hexy"
	}

	postbuildcommands {
			'{COPY} "../%{prj.name}/assets" "%{cfg.targetdir}/assets"',
			'{COPY} "%{cfg.targetdir}/GamePlayer.exe" "../Editor/EditorResources/"',
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"




workspace "Scripting"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}

	flags {
		"MultiProcessorCompile"
	}

project "ScriptCore"
	location "ScriptCore"
	kind "SharedLib"
	language "C#"

	targetdir ("Editor/assets/scripts/")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.cs",
	}

project "ScriptCustom"
	location "ScriptCustom"
	kind "SharedLib"
	language "C#"

	targetdir ("Editor/assets/scripts/")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.cs",
	}

	links {
		"ScriptCore"
	}