#include <PA9.h>

#define PA_UPDATEPAD(pad, var) do{ \
		Pad.pad.A = (var & KEY_A) != 0; \
		Pad.pad.B = (var & KEY_B) != 0; \
		Pad.pad.X = (var & KEY_X) != 0; \
		Pad.pad.Y = (var & KEY_Y) != 0; \
		Pad.pad.L = (var & KEY_L) != 0; \
		Pad.pad.R = (var & KEY_R) != 0; \
		Pad.pad.Select = (var & KEY_SELECT) != 0; \
		Pad.pad.Start = (var & KEY_START) != 0; \
		Pad.pad.Up = (var & KEY_UP) != 0; \
		Pad.pad.Down = (var & KEY_DOWN) != 0; \
		Pad.pad.Left = (var & KEY_LEFT) != 0; \
		Pad.pad.Right = (var & KEY_RIGHT) != 0; \
		Pad.pad.Anykey = (var & 0xFFF) != 0; \
	}while(0)

vu8 _PA_LidDown;

void PA_UpdatePad() {
	// Read the pad using libnds
	scanKeys();
	u32 k_held = keysHeld();
	u32 k_down = keysDown();
	u32 k_up   = keysUp();
	// Update the structures
	PA_UPDATEPAD(Held, k_held);
	PA_UPDATEPAD(Newpress, k_down);
	PA_UPDATEPAD(Released, k_up);
	// This also updates part of the stylus...
	Stylus.Held = (k_held & KEY_TOUCH) != 0;
	Stylus.Newpress = (k_down & KEY_TOUCH) != 0;
	Stylus.Released = (k_up & KEY_TOUCH) != 0;
	// ...and the lid.
	_PA_LidDown = (k_held & KEY_LID) != 0;
}

void PA_UpdateStylus() {
	touchPosition t_data;
	touchRead(&t_data);

	if(t_data.z1 != 0)
		Stylus.Pressure = (((t_data.px * t_data.z2) >> 6) / t_data.z1) - (t_data.px >> 6);
	else
		Stylus.Pressure = -1;

	Stylus.DblClick = Stylus.Newpress && ((Stylus.Downtime + Stylus.Uptime) < 45);
	Stylus.Downtime *= !Stylus.Newpress; // = 0 if newpress
	Stylus.Downtime += Stylus.Held;
	Stylus.Uptime *= !Stylus.Released; // = 0 when released
	Stylus.Uptime += !Stylus.Held;

	if (Stylus.Held) { // On en met à jour que si on touche l'écran, histoire de pas avoir un truc faussé
		Stylus.altX = (t_data.rawx - 0x0113) / 14;
		Stylus.altY = (t_data.rawy - 0x00E0) / 19;

		if (Stylus.Newpress) {
			Stylus.Vx = Stylus.oldVx = 0;
			Stylus.Vy = Stylus.oldVy = 0;
		} else {
			Stylus.oldVx = Stylus.Vx;
			Stylus.oldVy = Stylus.Vy;
			Stylus.Vx = t_data.px - Stylus.X;
			Stylus.Vy = t_data.py - Stylus.Y;
		}

		Stylus.X = t_data.px;
		Stylus.Y = t_data.py;
	}
}
