function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

function includeCatch()
	-- Catch is header-only, we need just the Catch.hpp header
	includedirs "Libraries/Catch/Include"
	
	-- We can also configure Catch through defines
	defines "CATCH_CPP11_OR_GREATER"
end

function useExampleLib()
	-- The library's public headers
	includedirs "projects/ecs/include"
	
	-- We link against a library that's in the same workspace, so we can just
	-- use the project name - premake is really smart and will handle everything for us.
	links "ECSLib"
	
	-- Users of ExampleLib need to link GLFW
	--linkGLFW()
end

workspace "ECS"
  location "generated"
  language "C++"
  architecture "x86_64"
  configurations { "debug", "release" }

  filter { "configurations:debug" }
  	symbols "On"

  filter { "configurations:release" }
  	optimize "On"

  filter {}

  targetdir "build/bin/%{prj.name}/%{cfg.longname}"
  objdir "build/obj/%{prj.name}/%{cfg.longname}"
  filter {"system:windows", "action:vs*"}
    systemversion(os.winSdkVersion() .. ".0")

project "ECSLib"
  kind "StaticLib"

  files "projects/ecslib/**"

project "ECSLibTest"
  kind "ConsoleApp"

  files "projects/ecslibtest/**"

  includeCatch()
