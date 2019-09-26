workspace "physics"
	configurations { "Debug", "DebugOptimized", "Release"}
	architecture "x64"
	filter {}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	inc = {}
	inc["debug"] = "%{wks.location}/debug/src"
	inc["physics"] = "%{wks.location}/physics/src"

	startproject "main"
	include "debug"
	include "physics"
	include "test"
	include "main"
    