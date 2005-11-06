#include <PA9.h>




u32 DX,DY;
u16 PA,PB,PC,PD;
s32 center_y, center_x;

u16 PAtemp; // temp value

u8 PA_currentmode;

s32 mode7cam_x, mode7cam_y, mode7cam_z, mode7alpha;
s32 mode7cos, mode7sin;	// temporaries for cos and sin alpha
s16 mode7_distance;




/*! \fn void PA_InitMode7(u8 bg_select)
    \brief
         \~english Initialize Mode 7 for a given background. You MUST be in video mode 1 or 2
         \~french Initialise le Mode 7 pour un fond donné. Vous devez etre en mode 1 ou 2 impérativement !
    \param bg_select
         \~english Bg number, 2 in mode 1, 2 or 3 in mode 2
         \~french Numéro du fond. 2 en mode 1, 2 ou 3 en mode 2
*/

void PA_InitMode7(u8 bg_select) {

mode7cam_x = X0; mode7cam_y = Y0; mode7cam_z = Z0;
mode7alpha = 0;
mode7cos = 256; mode7sin = 0;	// temporaries for cos and sin alpha
mode7_distance = 160;

PA_StartInt(INT_HBLANK, (void*)hbl_mode7);

}




// globals
// DIV:  a 160-LUT with 1/(h+1) ;  h=[0,160> (16.16 FIXEDs)
// g_cc:  sine value for current alpha (24.8 FIXEDs)
// g_ss:  cosine value for current alpha (24.8 FIXEDs)
// M7_D:   the projection plane distance (=160 in our case)
// cam_x, cam_y, cam_z: camera coordinates (24.8 FIXEDs)



u8 m7_distance = 160;

u8 PAcount;

void hbl_mode7(void) {
bool screen = 0;
u8 bg = 3;

	s32 ww, wcc, wss, wxr, wyr;


//	ww= (mode7cam_y*DIV[REG_VCOUNT])>>12;		// .8*.16 /.12 = 20.12
	ww= (mode7cam_y<<4)/(REG_VCOUNT+1);		// .8*.16 /.12 = 20.12
	wcc= ww*mode7cos>>5;					// .12*.8 /.5 = 17.15
	wss= ww*mode7sin>>5;					// .12*.8 /.5 = 17.15

	// NOTE: when you do P * q0
	// - for x-offset (q0_x): >>7, then *120
	// - for y-offset (q0_y): *160, then >>7
	// Bad things happen if you don't.

	// horizontal scale and offset
	wxr= 128*(wcc>>7);				// .8
	wyr= (mode7_distance*wss)>>7;				// .8
	PA_BGXPA(screen, bg)= wcc>>7;
	PA_BGXX(screen, bg)= mode7cam_x - wxr + wyr;

	// vertical scale and offset
	wxr= 128*(wss>>7);				// .8
	wyr= (mode7_distance*wcc)>>7;				// .8
	PA_BGXPC(screen, bg)= wss>>7;
	PA_BGXY(screen, bg)= mode7cam_z - wxr - wyr;

	// REG_IF = INT_HBLANK;		// cleared by isr switchblock
	
}
/*
PA_BGXX(screen, bg) = DX; // REG_BGXX
PA_BGXY(screen, bg) = DY; // REG_BGXY

PA_BGXPA(screen, bg) = PA;  //REG_BGXPA
PA_BGXPB(screen, bg) = PB;  //REG_BGXPB
PA_BGXPC(screen, bg) = PC;  //REG_BGXPC
PA_BGXPD(screen, bg) = PA;  //REG_BGXPD
*/
