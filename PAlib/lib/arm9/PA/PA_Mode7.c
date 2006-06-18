#ifdef __cplusplus
extern "C" {
#endif


#include <PA9.h>




u32 DX,DY;
u16 PA,PB,PC,PD;
s32 center_y, center_x;

u16 PAtemp; // temp value

u8 PA_currentmode;

s32 mode7cam_x, mode7cam_y, mode7cam_z, mode7alpha;
s32 mode7cos, mode7sin;	// temporaries for cos and sin alpha
s16 mode7_distance;

// in mode7.c
M7CAM _m7_cam;
int _m7_horizon= M7_TOP;
BGAFF_EX _m7_bgaff_ex[192];  // affine parameters for each line




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

//PA_StartInt(INT_HBLANK, hbl_mode7);
	irqSet(IRQ_HBLANK, hbl_mode7);
	irqEnable(IRQ_HBLANK);
}




// globals
// DIV:  a 160-LUT with 1/(h+1) ;  h=[0,160> (16.16 FIXEDs)
// g_cc:  sine value for current alpha (24.8 FIXEDs)
// g_ss:  cosine value for current alpha (24.8 FIXEDs)
// M7_D:   the projection plane distance (=160 in our case)
// cam_x, cam_y, cam_z: camera coordinates (24.8 FIXEDs)



u8 m7_distance = 160;

u8 PAcount;

void hbl_mode7(void){
u8 screen = 0;
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


#define INT_MIN 0
#define INT_MAX 2147483647

void m7_horz_calc(void){
    if(_m7_cam.v.y != 0)
    {
        _m7_horizon= M7_FAR_BG*_m7_cam.w.y - _m7_cam.pos.y;
		_m7_horizon= M7_TOP - swiDivide(_m7_horizon<<M7_D_SH, M7_FAR_BG*_m7_cam.v.y);
//        _m7_horizon= M7_TOP - swiDivide(horz<<M7_D_SH, M7_FAR_BG*_m7_cam.v.y);
    }
    else
        _m7_horizon= _m7_cam.w.y > 0 ? INT_MIN : INT_MAX;
}


/*
typedef struct { 
	s16 pa, pb, pc, pd; s32 dx, dy; 
} BGAFF_EX;
*/
/*#define PA_BGXX(screen, bg) _REG32(0x4000008 + (0x1000 * screen) + (bg << 4))
#define PA_BGXY(screen, bg) _REG32(0x400000C + (0x1000 * screen) + (bg << 4))
#define PA_BGXPA(screen, bg) _REG16(0x4000000 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPB(screen, bg) _REG16(0x4000002 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPC(screen, bg) _REG16(0x4000004 + (0x1000 * screen) + (bg << 4))
#define PA_BGXPD(screen, bg) _REG16(0x4000006 + (0x1000 * screen) + (bg << 4))
*/
void m7_hbl_flr(void){
u8 screen = 0;
u8 bg = 3;

    u32 vc = REG_VCOUNT;
	// Copie les trucs de BGAFF_EX
	PA_BGXPA(screen, bg) = _m7_bgaff_ex[vc].pa;
	PA_BGXPB(screen, bg) = _m7_bgaff_ex[vc].pb;
	PA_BGXPC(screen, bg) = _m7_bgaff_ex[vc].pc;
	PA_BGXPD(screen, bg) = _m7_bgaff_ex[vc].pd;	
	PA_BGXX(screen, bg) = _m7_bgaff_ex[vc].dx;
	PA_BGXY(screen, bg) = _m7_bgaff_ex[vc].dy;	
    //bga_ex_mem[2] = _m7_bgaff_ex[vc];
	
	// Faire le blending...
    //vid_bld_y( _m7_bgaff_ex[vc].pb>>6 );
}



void m7_aff_calc(void){
    if(_m7_horizon >= 192)
        return;

    int ii, ii0= _m7_horizon>=0 ? _m7_horizon : 0;
    s32 dcx= _m7_cam.v.x, dcy= _m7_cam.v.y, dcz= _m7_cam.v.z;
    s32 lam= INT_MAX, cx, cy, cz;

    // initial offsets
    cx= (_m7_cam.w.x<<M7_D_SH) + (ii0 - M7_TOP)*dcx;
    cy= (_m7_cam.w.y<<M7_D_SH) + (ii0 - M7_TOP)*dcy;
    cz= (_m7_cam.w.z<<M7_D_SH) + (ii0 - M7_TOP)*dcz;
    BGAFF_EX *bga= &_m7_bgaff_ex[ii0];

    for(ii=ii0; ii<192; ii++)
    {
		if (cy == 0) lam = INT_MAX;
		else lam= swiDivide(_m7_cam.pos.y<<12, cy);    // .12

        // a little hack
        bga->pb= lam>>4;         // .8

        // calculate affine parameters
        bga->pa= (lam*_m7_cam.u.x)>>12;
        bga->dx= _m7_cam.pos.x + M7_LEFT*bga->pa - (lam*cx>>12);

        bga->pc= (lam*_m7_cam.u.z)>>12;
        bga->dy= _m7_cam.pos.z + M7_LEFT*bga->pc - (lam*cz>>12);

        cx += dcx;      cy += dcy;      cz += dcz;
        bga++;
    }
}


#ifdef __cplusplus
}
#endif



