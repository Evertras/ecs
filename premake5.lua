workspace "ECS"
  location "Generated"
  language "C++"
  architecture "x86_64"
  configurations ["debug", "release"]

  filter { "configurations:debug" }
  	flags { "Symbols" }

  filter { "configurations:release" }
  	optimize "On"

  filter {}

  targetdir { "build/bin/%{prj.name}/%{cfg.longname}" }
  objdir { "build/obj/%{prj.name}/%{cfg.longname}"}
