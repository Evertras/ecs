function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

function includeCatch()
	includedirs "libraries/catch/include"
	defines "CATCH_CPP11_OR_GREATER"
end

function useECSLib()
	includedirs "projects/ecslib/include"
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
		links "GLEW"

	filter {}
end

function useOpenGL()
	filter "system:windows"
		links "OpenGL32"

	filter "system:linux"
		links "GL"

    filter {}
end

function useGLM()
  includedirs "libraries/glm/include"
  defines { "GLM_ENABLE_EXPERIMENTAL" }
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

  filter {"system:windows", "action:vs*"}
    systemversion(os.winSdkVersion() .. ".0")

  filter { "action:gmake*" }
    buildoptions { "-std=c++17" }

  filter {}

  targetdir "build/bin/%{prj.name}/%{cfg.longname}"
  objdir "build/obj/%{prj.name}/%{cfg.longname}"

  startproject "Sample"

project "ECSLib"
  kind "StaticLib"

  files "projects/ecslib/**"

project "ECSLibTest"
  kind "ConsoleApp"
  files "projects/ecslibtest/**"
  includeCatch()
  useECSLib()

  filter "action:gmake*"
    --prebuildcommands {
      --"@echo BEFORE"
    --}

    postbuildcommands {
      -- Run the tests
      "@%{cfg.buildtarget.relpath}"
    }

project "Sample"
  kind "ConsoleApp"

  pchheader "pch.h"
  pchsource "projects/sample/pch.cpp"
  --includedirs "projects/sample"

  files "projects/sample/**"
  debugdir "projects/sample"

  -- Internal
  useECSLib()

  -- Third party
  useSDL()
  useSOIL()
  useGlew()
  useOpenGL()
  useGLM()

  filter "action:vs*"
    prebuildcommands {
      '..\\bin\\windows\\astyle.exe ..\\projects\\sample\\*.cpp --recursive --options=../.astylerc',
      '..\\bin\\windows\\astyle.exe ..\\projects\\sample\\*.h --recursive --options=../.astylerc'
    }

  filter "action:gmake*"
	  prebuildcommands {
	    '@echo Copying static assets',
	    'cp -R ../projects/sample/assets %{cfg.buildtarget.directory}'
	  }

  filter {}

project "SampleTest"
  kind "ConsoleApp"
  files "projects/sampletest/**"
  includeCatch()
  useECSLib()
  useSDL()
  useSOIL()
  useGlew()
  useOpenGL()
  useGLM()

  includedirs "projects/sample/"
  links "Sample"

  filter "action:gmake*"
    --prebuildcommands {
      --"@echo BEFORE"
    --}

    postbuildcommands {
      -- Run the tests
      "@%{cfg.buildtarget.relpath}"
    }
