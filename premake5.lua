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

	libdirs "libraries/SDL/lib/%{cfg.system}/%{cfg.architecture}"
	
	links "SDL2"
	links "SDL2main"
end

function useSOIL()
	includedirs "libraries/SOIL/src"

	libdirs "libraries/SOIL/lib/%{cfg.system}/%{cfg.architecture}"
	
	links "SOIL"
end

function useGlew()
	includedirs "libraries/glew/include"

	libdirs "libraries/glew/lib/%{cfg.system}/%{cfg.architecture}"

  filter "system:windows"
    links "glew32"
    links "glew32s"

  filter "system:linux"
    links "libGLEW"
end

function useOpenGL()
  links "OpenGL32"
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
    --systemversion(os.winSdkVersion() .. ".0")

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

  -- Internal
  useECSLib()

  -- Third party
  useSDL()
  useSOIL()
  useGlew()
  useOpenGL()

  includedirs "libraries/glm/include"

  --pchheader "pch.h"
  --pchsource "pch.cpp"
  --includedirs "projects/sample"

