#ifdef __cplusplus
extern "C" {
#endif


#include <PA9.h>
#include <nds/arm9/ndsmotion.h>
/*
#define CARD_WaitBusy()   while (CARD_CR1 & 0x80);

// enables SPI bus at 4.19 MHz
#define SPI_On() CARD_CR1 = 0x8000 |0x2000 | 0x40 | 0;

// disables SPI bus
#define SPI_Off() CARD_CR1 = 0;
*/

extern funcpointer MotionVBL; // VBL function... set to nothing by default
motion_struct Motion;

// global variables for the sensor parameters
int PA_Xoffset = 2048; // output of X axis at 0 acceleration - this can vary slightly from sensor to sensor
int PA_Yoffset = 2048; // output of Y axis at 0 acceleration - this can vary slightly from sensor to sensor
int PA_Zoffset = 2048; // output of Z axis at 0 acceleration - this can vary slightly from sensor to sensor
int PA_Xsens = 819; // sensitivity of X axis (how many counts the output changes with 1 g) - this can vary slightly from sensor to sensor
int PA_Ysens = 819; // sensitivity of Y axis (how many counts the output changes with 1 g) - this can vary slightly from sensor to sensor
int PA_Zsens = 819; // sensitivity of Z axis (how many counts the output changes with 1 g) - this can vary slightly from sensor to sensor
int PA_Goffset = 1680; // zero rate bias (offset) of the gyro - this can vary slightly from sensor to sensor
int PA_Gsens = 825;// sensitivity of gyro (how many counts the output changes with 1000 degrees/second rotation) - this can vary slightly from sensor to sensor

void PA_MotionVBL(void);




u8 pamotiontopad;

typedef struct {
	s32 X, Y, Z;
}pamotiontype;
pamotiontype pamotionold[4];
u8 pamotioncurrent;

void PA_MotionVBL(void){
	// Backup
	s32 oldx = Motion.X;
	s32 oldy = Motion.Y;
	s32 oldz = Motion.Z;
	s32 oldzrot = Motion.Zrot;   
		
	// Read commands
   Motion.X = motion_read_x(); // this returns a value between 0 and 4095
	Motion.Y = motion_read_y(); // this returns a value between 0 and 4095
	Motion.Z = motion_read_z(); // this returns a value between 0 and 4095	
   Motion.AccelX = motion_acceleration_x(); // this returns a value between 0 and 4095
	Motion.AccelY = motion_acceleration_y(); // this returns a value between 0 and 4095
	Motion.AccelZ = motion_acceleration_z(); // this returns a value between 0 and 4095	
	
	Motion.Zrot = motion_read_gyro(); // this returns a value between 0 and 4095
	Motion.X = ((Motion.X-PA_Xoffset)*1000/PA_Xsens)>>3; // this converts from counts to mg (milli-g) (1000 mg = 1 g)
	Motion.Y = ((Motion.Y-PA_Yoffset)*1000/PA_Ysens)>>3; // 1 g is gravitational acceleration
	Motion.Z = ((Motion.Z-PA_Zoffset)*1000/PA_Zsens)>>3;
	Motion.Zrot = (Motion.Zrot - PA_Goffset)*1000/PA_Gsens; // this converts to deg/sec (degrees per second)  

	Motion.Newpress.Left = (!(oldx > 20)) && (Motion.X > 20);
	Motion.Newpress.Right = (!(oldx < -20)) && (Motion.X < -20);		
	Motion.Newpress.Down = (!(oldy > 20)) && (Motion.Y > 20);
	Motion.Newpress.Up = (!(oldy < -20)) && (Motion.Y < -20);

	Motion.Released.Left = (oldx > 20) && !(Motion.X > 20);
	Motion.Released.Right = (oldx < -20) && !(Motion.X < -20);		
	Motion.Released.Down = (oldy > 20) && !(Motion.Y > 20);
	Motion.Released.Up = (oldy < -20) && !(Motion.Y < -20);
		
	Motion.Held.Left = (Motion.X > 20);
	Motion.Held.Right = (Motion.X < -20);		
	Motion.Held.Down = (Motion.Y > 20);
	Motion.Held.Up = (Motion.Y < -20);	
	
	Motion.Angle = PA_GetAngle(0, 0, -Motion.X, Motion.Y);
	Motion.Force = PA_TrueDistance(0, 0, Motion.X, Motion.Y);
	
	if(pamotiontopad){ // Map 
		Pad.Newpress.Left = Motion.Newpress.Left;
		Pad.Newpress.Up = Motion.Newpress.Up;
		Pad.Newpress.Down = Motion.Newpress.Down;
		Pad.Newpress.Right = Motion.Newpress.Right;

		Pad.Held.Left = Motion.Held.Left;
		Pad.Held.Up = Motion.Held.Up;
		Pad.Held.Down = Motion.Held.Down;
		Pad.Held.Right = Motion.Held.Right;

		Pad.Released.Left = Motion.Released.Left;
		Pad.Released.Up = Motion.Released.Up;
		Pad.Released.Down = Motion.Released.Down;
		Pad.Released.Right = Motion.Released.Right;	
	}
	
	// Get Changes...
	Motion.Vx = Motion.X - oldx;
	Motion.Vy = Motion.Y - oldy;
	Motion.Vz = Motion.Z - oldz;
	Motion.Vzrot = Motion.Zrot - oldzrot;	 
	
	pamotioncurrent++; pamotioncurrent&=3; // limit to 4...
	
	pamotionold[pamotioncurrent].X = Motion.X; 
	pamotionold[pamotioncurrent].Y = Motion.Y; 
	pamotionold[pamotioncurrent].Z = Motion.Z; 	
	
	Motion.X = (pamotionold[0].X + pamotionold[1].X + pamotionold[2].X + pamotionold[3].X)>>2; // Average value
	Motion.Y = (pamotionold[0].Y + pamotionold[1].Y + pamotionold[2].Y + pamotionold[3].Y)>>2; // Average value	
	Motion.Z = (pamotionold[0].Z + pamotionold[1].Z + pamotionold[2].Z + pamotionold[3].Z)>>2; // Average value		
}   



// read the X acceleration



#ifdef __cplusplus
}
#endif



