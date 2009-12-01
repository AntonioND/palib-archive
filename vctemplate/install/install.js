////////////////////////////////////
// PAlibAppTemplate installer 1.0 //
// copyright (c) fincs            //
////////////////////////////////////
main();

function main(){
	var oFS = WScript.CreateObject("Scripting.FileSystemObject");
	var oSh = WScript.CreateObject("WScript.Shell");
	var curpath = oFS.GetAbsolutePathName(".");

	WScript.Echo("PAlibAppTemplate 1.0\nby fincs");

	var vc9exp_key = "HKLM\\Software\\Microsoft\\VCExpress\\9.0\\Setup\\VC\\ProductDir";
	var vc9_key = "HKLM\\Software\\Microsoft\\VisualStudio\\9.0\\Setup\\VC\\ProductDir";
	var vc9exp_key2 = "HKLM\\SOFTWARE\\Wow6432Node\\Microsoft\\VCExpress\\9.0\\Setup\\VC\\ProductDir";
	var vc9_key2 = "HKLM\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\9.0\\Setup\\VC\\ProductDir";
	var vc8exp_key = "HKLM\\Software\\Microsoft\\VCExpress\\8.0\\Setup\\VC\\ProductDir";
	var vc8_key = "HKLM\\Software\\Microsoft\\VisualStudio\\8.0\\Setup\\VC\\ProductDir";
	var vc8exp_key2 = "HKLM\\SOFTWARE\\Wow6432Node\\Microsoft\\VCExpress\\8.0\\Setup\\VC\\ProductDir";
	var vc8_key2 = "HKLM\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\8.0\\Setup\\VC\\ProductDir";
	var isExpress = true;
	var isVC9 = true;
	var vcdir;

	// Read the VC++ 2005/2008 installation path using a REALLY long try..catch block...

	try{
		vcdir = oSh.RegRead(vc9exp_key);
	}catch(e){
		try{
			vcdir = oSh.RegRead(vc9exp_key2);
		}catch(e){
			try{
				isExpress = false;
				vcdir = oSh.RegRead(vc9_key);
			}catch(e){
				try{
					vcdir = oSh.RegRead(vc9_key2);
				}catch(e){
					try{
						isExpress = true;
						isVC9 = false;
						vcdir = oSh.RegRead(vc8exp_key);
					}catch(e){
						try{
							vcdir = oSh.RegRead(vc8exp_key2);
						}catch(e){
							try{
								isExpress = false;
								vcdir = oSh.RegRead(vc8_key);
							}catch(e){
								try{
									vcdir = oSh.RegRead(vc8_key2);
								}catch(e){
									WScript.Echo("Can't determine the Visual C++ 2008/2005 installation path!");
									return;
								}
							}
						}
					}
				}
			}
		}
	}

	// Strip the trailing backslash
	vcdir = vcdir.substring(0, vcdir.length-1);

	try{
		var pdfolder = vcdir + (isExpress ? "\\Express\\VCProjects" : "\\vcprojects");
		if(!oFS.FolderExists(pdfolder)){
			WScript.Echo("Can't find folder:\n" + pdfolder);
			return;
		}

		var pdfolder_ngc = pdfolder + "\\Nintendo Game Console";
		createFolder(oFS, pdfolder_ngc);

		var wizfolder = vcdir + "\\VCWizards\\AppWiz";
		if(!oFS.FolderExists(wizfolder)){
			WScript.Echo("Can't find folder:\n" + wizfolder);
			return;
		}

		var wizfolder_ngc = wizfolder + "\\Nintendo Game Console";
		createFolder(oFS, wizfolder_ngc);

		var wizfolder_dest = wizfolder_ngc + "\\PAlibAppTemplate";
		createFolder(oFS, wizfolder_dest);

		createFolder(oFS, wizfolder_dest + "\\HTML");
		createFolder(oFS, wizfolder_dest + "\\HTML\\1033");
		createFolder(oFS, wizfolder_dest + "\\Images");
		createFolder(oFS, wizfolder_dest + "\\Scripts");
		createFolder(oFS, wizfolder_dest + "\\Scripts\\1033");

		installReferences(oFS, curpath, pdfolder, pdfolder_ngc, isVC9);
		installWizard(oFS, curpath, wizfolder_dest);
	}catch(e){
		WScript.Echo("Installation failed!\nMake sure you run this script with administrator privileges.");
		return;
	}

	WScript.Echo("Installation completed successfully for Visual C++ 200" + (isVC9 ? "8" : "5") + " " + (isExpress ? "Express " : "") + ":)");
}

function createFolder(oFS, name){
	try{
		if(!oFS.FolderExists(name)){
			oFS.CreateFolder(name);
		}
	}catch(e){
		throw e;
	}
}

function installReferences(oFS, src, dest, dest2, vc9){
	try{
		oFS.CopyFile(src + "\\PAlibAppTemplate" + (vc9 ? "9" : "8") + ".vsz", dest + "\\PAlibAppTemplate.vsz");
		oFS.CopyFile(src + "\\PAlibAppTemplate.ico", dest + "\\");
		oFS.CopyFile(src + "\\PAlibAppTemplate.vsdir", dest2 + "\\");
	}catch(e){
		throw e;
	}
}

function installWizard(oFS, src, dest){
	try{
		oFS.CopyFile(src + "\\default.htm", dest + "\\HTML\\1033\\");
		oFS.CopyFile(src + "\\default.js", dest + "\\Scripts\\1033\\");
	}catch(e){
		throw e;
	}
}
