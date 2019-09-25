workspace "template"
	configurations { "Debug", "DebugOptimized", "Release"}
	architecture "x64"
	filter {}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	inc = {}
	inc["debug"] = "%{wks.location}/debug/src"

	startproject "main"
	include "debug"
	include "test"
	include "main"
    