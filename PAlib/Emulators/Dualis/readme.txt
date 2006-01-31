		-------------------------------------------------------
				        Dualis
			             A DS emulator

				      /Mic, 2005

		-------------------------------------------------------



	About
	-----
	
	Dualis is a plugin-based DS emulator for x86/Win32 computers. It is
	written	in x86 assembly and C++.

	This version mainly supports ARM9 binaries, which are loaded into
	main memory at 0x2004000 and excuted. The nds file format is also
	supported, in which case the ARM9 code is loaded into the address
	specified in the header.


	What is emulated
	----------------

	ARM946E-S	Most armv4 features, some armv5te features.
			Caches are currently disabled.

	ARM7TDMI	Nothing.

	Video		Both screens are emulated.
			The following modes are emulated (more or less):
			
			  BG_MODE0
			  BG_MODE1
			  BG_MODE2
			  BG_MODE3
			  BG_MODE4
			  BG_MODE5
			  BG_MODE6	 (main core)
			  MODE_FB0..FB3	 (main core)

			All color effects are emulated (except for alpha-
			blending on BGs using extended palettes).
			The 3D-hardware commands are mapped directly
			onto OpenGL. There are some problems with the
			matrix stacks in this release, which causes
			perspective errors in most 3D demos.

	Audio		Nothing.

	Input		All keys are emulated.
			Touchscreen input is faked using the IPC method
			implemented in NDSLib. 

	DMA		DM1, DM2 and DM3 are emulated.
	
	Timers		All timers are emulated in both prescalar and count-up mode.

	Interrupts	Vblank, hblank, key, DMA and timer interrupts are emulated,
			though not completely accurate.	


	Keys
	----

	D-Up		Up arrow
	D-Down		Down arrow
	D-Left		Left arrow
	D-Right		Right arrow
	B		Z
	A		X
	Start		Return
	Select		Space
	L		L
	R		R

	Touchscreen	Use the mouse to move the stylus around, and the left
			button to "touch" the screen.

	Ctrl+Shift+d toggles DMA transfers on/off.


	1		Screenshot main video core
	2		Screenshot sub video core
	3		Screenshot both video cores

	,		Start recording AVI
	.		Stop recording AVI
	Ctrl+a		Setup AVI recording


	Commandline parameters
	----------------------

	-theme <n>	Select color theme setting (n = 0..15)

	-wincon		Makes dualis open up a windows console where it 
			prints some information.
		