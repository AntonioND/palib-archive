//////////////////////////
// PAlibAppTemplate 1.0 //
// copyright (c) fincs  //
//////////////////////////

function OnFinish(selProj, selObj){
	try{
		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");

		var wshell = new ActiveXObject("WScript.Shell");
   	
        	var devkitPath = wshell.ExpandEnvironmentStrings("%DEVKITPRO%");
		devkitPath = devkitPath.charAt(1) + ":" + devkitPath.substring(2).replace(new RegExp("/", "g"), "\\");

        	var toolchainPath = wshell.ExpandEnvironmentStrings("%DEVKITARM%");
		toolchainPath = toolchainPath.charAt(1) + ":" + toolchainPath.substring(2).replace(new RegExp("/", "g"), "\\");
		
		selProj = CreateProject(strProjectName, strProjectPath);
		selProj.Object.Keyword = "MakeFileProj";
		
		var bSameAsDebug = true;
		
		var debugConfig = selProj.Object.Configurations("Debug");
		PrepareConfig(debugConfig, true, devkitPath, toolchainPath);

		var releaseConfig = selProj.Object.Configurations("Release");
		PrepareConfig(releaseConfig, false, devkitPath, toolchainPath);

		var f_source = selProj.Object.AddFilter("source");
		var f_include = selProj.Object.AddFilter("include");
		var f_data = selProj.Object.AddFilter("data");
		var f_gfx = selProj.Object.AddFilter("gfx");
		var f_audio = selProj.Object.AddFilter("audio");
		var f_filesystem = selProj.Object.AddFilter("filesystem");

		try{
			wizard.RenderTemplate(devkitPath + "\\PAlib\\vctemplate\\overrides\\ReadMe.txt", "ReadMe.txt", true, true);
			wizard.RenderTemplate(devkitPath + "\\PAlib\\template\\Makefile", "Makefile", true, true);
			wizard.RenderTemplate(devkitPath + "\\PAlib\\template\\source\\main.c", "source/main.c", true, true);
			wizard.RenderTemplate(devkitPath + "\\PAlib\\tools\\PAGfx\\PAGfx.exe", "gfx/PAGfx.exe", true, true);
			wizard.RenderTemplate(devkitPath + "\\PAlib\\tools\\PAGfx\\PAGfxFrontend.exe", "gfx/PAGfxFrontend.exe", true, true);
			f_source.AddFile("source/main.c");
			f_gfx.AddFile("gfx/PAGfxFrontend.exe");
			f_gfx.AddFile("gfx/PAGfx.exe");
			selProj.Object.AddFile("ReadMe.txt");
			selProj.Object.AddFile("Makefile");
		}catch(e){
			alert("Can't copy files!");
			window.external.Finish(document, 'cancel');
		}
        
		selProj.Object.Save();
	}catch(e){
		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number;
	}
}

function PrepareConfig(config, isDebug, devkitPath, toolchainPath){
	try{
		config.OutputDirectory = "$(ConfigurationName)";
		config.IntermediateDirectory = "$(ConfigurationName)";
		config.ConfigurationType = typeUnknown;
      	 	config.DebugSettings.Command ="cmd.exe";
		config.DebugSettings.CommandArguments = " /c start $(TargetPath)";

		var NMakeTool = config.Tools("VCNMakeTool");

		NMakeTool.BuildCommandLine = "make -r 2>&1 | sed -e 's/\\(.[a-zA-Z]\\+\\):\\([0-9]\\+\\):/\\1(\\2):/";
		NMakeTool.Output = "$(ProjectName).nds";
		NMakeTool.CleanCommandLine = "make clean";
		NMakeTool.ReBuildCommandLine = "make clean && make -r 2>&1 | sed -e 's/\\(.[a-zA-Z]\\+\\):\\([0-9]\\+\\):/\\1(\\2):/";

		NMakeTool.IncludeSearchPath = toolchainPath + "\\arm-eabi\\include;" +                 // C standard library
		                              toolchainPath + "\\lib\\gcc\\arm-eabi\\4.4.2\\include;" + // C standard library, part 2
		                              toolchainPath + "\\arm-eabi\\include\\c++\\4.4.2;" +     // C++ standard library
		                              devkitPath + "\\libnds\\include;" +                      // libnds include path
		                              devkitPath + "\\PAlib\\include\\nds";                    // PAlib include path

		var uDefines = "ARM9";
		if(!isDebug){ uDefines += ";NDEBUG"; }
		NMakeTool.PreprocessorDefinitions = uDefines;
	}catch(e){
		throw e;
	}
}
