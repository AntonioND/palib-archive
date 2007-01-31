#ifdef __cplusplus
extern "C" {
#endif


#include <PA9.h>

#define CARD_WaitBusy()   while (CARD_CR1 & /*BUSY*/0x80);

// enables SPI bus at 4.19 MHz
#define SPI_On() CARD_CR1 = /*E*/0x8000 | /*SEL*/0x2000 | /*MODE*/0x40 | 0;

// disables SPI bus
#define SPI_Off() CARD_CR1 = 0;


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

// sends and receives 1 byte on the SPI bus
unsigned char SPI(unsigned char in_byte)
{
	unsigned char out_byte;
	CARD_EEPDATA = in_byte; // send the output byte to the SPI bus
	CARD_WaitBusy(); // wait for transmission to complete
	out_byte=CARD_EEPDATA; // read the input byte from the SPI bus
	return out_byte;
}

// turn on the accelerometer
u8 PA_MotionInit(void) {
	WAIT_CR &= ~0x0880;	// DS Card access ARM9:bit11=0   GBA Cart access ARM9:bit7=0
	// send 0x04, 0x04
	SPI_On()
	SPI(0x04); // command to write to control register
	SPI(0x04); // enable
	SPI_Off()
    int enabled = 0;
    SPI_On()
    SPI(0x03); //command to read from KXB5 control register
    if( SPI(0x00) == 0x04) {
		enabled = 1; // control register equals 0x04 if enabled
		MotionVBL = PA_MotionVBL;   // Enable VBL function
	}
    SPI_Off();
    return enabled;
}



signed int readX();
signed int readY();
signed int readZ();
signed int readGyro();

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
   Motion.X = readX(); // this returns a value between 0 and 4095
	Motion.Y = readY(); // this returns a value between 0 and 4095
	Motion.Z = readZ(); // this returns a value between 0 and 4095		
	Motion.Zrot = readGyro(); // this returns a value between 0 and 4095
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
signed int readX() {
	signed int output = 0;
	SPI_On()
	SPI(0x00); // command to convert X axis
	swiDelay(625); // wait at least 40 microseconds for the A-D conversion
	output = ( (SPI(0x00)<<8)|SPI(0x00) )>>4; // read 16 bits and store as a 12 bit number
	SPI_Off()
	return output;
}

// read the Y acceleration
signed int readY() {
	signed int output = 0;
	SPI_On()
	SPI(0x02); // command to convert Y axis
	swiDelay(625); // wait at least 40 microseconds for the A-D conversion
	output = ( (SPI(0x00)<<8)|SPI(0x00) )>>4; // read 16 bits and store as a 12 bit number
	SPI_Off()
	return output;
}

// read the Z acceleration
signed int readZ() {
	signed int output = 0;
	SPI_On()
	SPI(0x01); // command to convert Z axis
	swiDelay(625); // wait at least 40 microseconds for the A-D conversion
	output = ( (SPI(0x00)<<8)|SPI(0x00) )>>4; // read 16 bits and store as a 12 bit number
	SPI_Off()
	return output;
}

// read the Z rotational speed
signed int readGyro() {
	signed int output = 0;
	SPI_On()
	SPI(0x07); // command to convert analog Auxilliary input (the gyro)
	swiDelay(625); // wait at least 40 microseconds for the A-D conversion
	output = ( (SPI(0x00)<<8)|SPI(0x00) )>>4; // read 16 bits and store as a 12 bit number
	SPI_Off()
	return output;
}

// checks whether a DS Motion Card is plugged in
u8 PA_CheckDSMotion(){
    u8 plugged_in = 1;
    SPI_On()
    SPI(0x03); //command to read from KXB5 control register
    // send dummy byte to receive contents of control register
    // if there is nothing plugged in, this will return 255
	// emulators will return 0
    // if the DS Motion Card is plugged in, it will return a smaller nonzero number
    int control_reg = SPI(0x00);
	if( control_reg == 255 || control_reg == 0) plugged_in = 0;
    SPI_Off()
    return plugged_in;
}




#ifdef __cplusplus
}
#endif



