# Building with Visual Studio:

### Run from the root directory:
```.\build\windows\build.bat```
### And open the solution file in `build\bin`

# Building with Visual Studio Code (Cross Platform Compatible):

### Example VSCode tasks.json:
```json
{
	"version": "2.0.0",
	"tasks": [
		{
			"label": "Win_BuildDebug",
			"type": "shell",
			"command": "./build/windows/build.bat Debug"
		},
		{
			"label": "Win_BuildDebugOpt",
			"type": "shell",
			"command": "./build/windows/build.bat RelWithDebInfo"
		},
		{
			"label": "Win_BuildMinSize",
			"type": "shell",
			"command": "./build/windows/build.bat MinSizeRel"
		},
		{
			"label": "Win_BuildRelease",
			"type": "shell",
			"command": "./build/windows/build.bat Release"
		},
		{
			"label": "Lin_BuildDebug",
			"type": "shell",
			"command": "./build/linux/build.sh Debug"
		},
		{
			"label": "Lin_BuildDebugOpt",
			"type": "shell",
			"command": "./build/linux/build.sh RelWithDebInfo"
		},
		{
			"label": "Lin_BuildMinSize",
			"type": "shell",
			"command": "./build/linux/build.sh MinSizeRel"
		},
		{
			"label": "Lin_BuildRelease",
			"type": "shell",
			"command": "./build/linux/build.sh Release"
		}
	]
}
```
### Example VSCode launch.json:
```json
{
	"version": "0.2.0",
	"configurations": [
		{
			"name": "Launch Debug (Windows)",
			"type": "cppvsdbg",
			"request": "launch",
			"preLaunchTask": "Win_BuildDebug",
			"program": "${workspaceFolder}/build/bin/main/Debug/main.exe",
			"args": [],
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false
		},
		{
			"name": "Launch Debug Optimized (Windows)",
			"type": "cppvsdbg",
			"request": "launch",
			"preLaunchTask": "Win_BuildDebugOpt",
			"program": "${workspaceFolder}/build/bin/main/RelWithDebInfo/main.exe",
			"args": [],
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false
		},
		{
			"name": "Launch Release Minimum Size (Windows)",
			"type": "cppvsdbg",
			"request": "launch",
			"preLaunchTask": "Win_BuildMinSize",
			"program": "${workspaceFolder}/build/bin/main/MinSizeRel/main.exe",
			"args": [],
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false
		},
		{
			"name": "Launch Release (Windows)",
			"type": "cppvsdbg",
			"request": "launch",
			"preLaunchTask": "Win_BuildRelease",
			"program": "${workspaceFolder}/build/bin/main/Release/main.exe",
			"args": [],
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false
		},
		{
			"name": "Launch Debug (Linux)",
			"type": "cppdbg",
			"request": "launch",
			"preLaunchTask": "Lin_BuildDebug",
			"program": "${workspaceFolder}/build/bin/main/main",
			"args": [],
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false,
			"MIMode": "gdb",
			"miDebuggerPath": "/usr/bin/gdb",
			"setupCommands": [
				{
					"description": "Enable pretty-printing for gdb",
					"text": "-enable-pretty-printing",
					"ignoreFailures": true
				}
			]
		},
		{
			"name": "Launch Debug Optimized (Linux)",
			"type": "cppdbg",
			"request": "launch",
			"preLaunchTask": "Lin_BuildDebugOpt",
			"program": "${workspaceFolder}/build/bin/main/main",
			"args": [],
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false,
			"MIMode": "gdb",
			"miDebuggerPath": "/usr/bin/gdb",
			"setupCommands": [
				{
					"description": "Enable pretty-printing for gdb",
					"text": "-enable-pretty-printing",
					"ignoreFailures": true
				}
			]
		},
		{
			"name": "Launch Release Minimum Size (Linux)",
			"type": "cppdbg",
			"request": "launch",
			"preLaunchTask": "Lin_BuildMinSize",
			"program": "${workspaceFolder}/build/bin/main/main",
			"args": [],
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false,
			"MIMode": "gdb",
			"miDebuggerPath": "/usr/bin/gdb",
			"setupCommands": [
				{
					"description": "Enable pretty-printing for gdb",
					"text": "-enable-pretty-printing",
					"ignoreFailures": true
				}
			]
		},
		{
			"name": "Launch Release (Linux)",
			"type": "cppdbg",
			"request": "launch",
			"preLaunchTask": "Lin_BuildRelease",
			"program": "${workspaceFolder}/build/bin/main/main",
			"args": [],
			"stopAtEntry": false,
			"cwd": "${workspaceFolder}",
			"environment": [],
			"externalConsole": false,
			"MIMode": "gdb",
			"miDebuggerPath": "/usr/bin/gdb",
			"setupCommands": [
				{
					"description": "Enable pretty-printing for gdb",
					"text": "-enable-pretty-printing",
					"ignoreFailures": true
				}
			]
		}
	]
}
```
