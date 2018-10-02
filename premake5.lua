function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

function includeCatch()
	includedirs "Libraries/Catch/Include"
	defines "CATCH_CPP11_OR_GREATER"
end

function useECSLib()
	includedirs "projects/ecslib/include"
	links "ECSLib"
end

function useSDL()
	includedirs "libraries/SDL/include"

	-- TODO: figure out how to change between windows/linux when we get linux libs in
	libdirs "libraries/SDL/lib/windows/%{cfg.architecture}"
	
	links "SDL2"
	links "SDL2main"
end

function useSOIL()
	includedirs "libraries/SOIL/src"

	-- TODO: figure out how to change between windows/linux when we get linux libs in
	libdirs "libraries/SOIL/lib/windows/%{cfg.architecture}"
	
	links "SOIL"
end

function useGlew()
	includedirs "libraries/glew/include"

	-- TODO: figure out how to change between windows/linux when we get linux libs in
	libdirs "libraries/glew/lib/windows/%{cfg.architecture}"

	links "glew32"
	links "glew32s"
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

  startproject "Sample"

project "ECSLib"
  kind "StaticLib"

  files "projects/ecslib/**"

project "ECSLibTest"
  kind "ConsoleApp"
  files "projects/ecslibtest/**"
  includeCatch()
  useECSLib()

project "Sample"
  kind "ConsoleApp"
  files "projects/sample/**"
  debugdir "projects/sample"
  useECSLib()
  useSDL()
  useSOIL()
  useGlew()
  links "OpenGL32.lib"

  includedirs "libraries/glm/include"

  --pchheader "pch.h"
  --pchsource "pch.cpp"
  --includedirs "projects/sample"
