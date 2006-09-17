#ifndef _PA_Interrupt
#define _PA_Interrupt

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_Interrupt.h
    \brief Interrupt system

    Current working interrupts are VBlank, HBlank, VCount, and Timers
*/

typedef void(*funcpointer)(void);

extern volatile u8 PA_Newframe;

extern volatile u8 PA_vblok; // Passe à 1 quand VBL activé...


extern u32 PA_CPU; // Pourcentage CPU...
extern u32 PA_MaxCPU; // Max CPU...
extern u32 PA_lines; // Nombre de lignes, pour faire le calcul...
extern u8 PA_VBLCount; // Compte le nombre de VBL avant qu'on en arrive à un tour
extern u8 PA_nVBLs;
extern s32 PA_VBLCounter[16];  // VBL counters available
extern bool PA_VBLCounterOn[16];  // On or off

extern funcpointer CustomVBL;

extern volatile s32 PA_TestVBLs;

/*
//Interrupt Registers
#ifndef REG_IME
	#define REG_IME *(u32*)0x04000208
#endif
#ifndef REG_IE
#define REG_IE *(u32*)0x04000210
#endif
#ifndef REG_IF
#define REG_IF *(u32*)0x04000214
#endif*/

/*
#define REG_INTERRUPT IRQ_HANDLE
#define REG_DISPSTAT DISP_SR
*/

//#define REG_VCOUNT _REG16(0x04000006)
/*
// Interrupts
#define INTVBLANK 0x0001
#define INTHBLANK 0x0002
#define INTVCOUNT 0x0004
#define INTTIMER0 0x0008
#define INTTIMER1 0x0010
#define INTTIMER2 0x0020
#define INTTIMER3 0x0040
#define INTCOM 0x0080
#define INTDMA0 0x0100
#define INTDMA1 0x0200
#define INTDMA2 0x0400
#define INTDMA3 0x0800
#define INTBUTTON 0x1000
#define INTCART 0x2000

#define INT_VBLANK 0
#define INT_HBLANK 1
#define INT_VCOUNT 2
#define INT_TIMER0 3
#define INT_TIMER1 4
#define INT_TIMER2 5
#define INT_TIMER3 6
#define INT_COM 7
#define INT_DMA0 8
#define INT_DMA1 9
#define INT_DMA2 10
#define INT_DMA3 11
#define INT_KEY 12
#define INT_CART 13*/

#define ENABLE_VBLANK 8
#define ENABLE_HBLANK 16
#define ENABLE_VCOUNT 32

/*
// Pour activer les timers, que pour moi...
// TIMER O
#define TIMER0COUNT		*(volatile u16 *)(0x04000100)
#define TIMER0CNT		*(volatile u16 *)(0x04000102)
#define TIMER0CNT_TIMER_START               (TIMER0CNT |= 0x0080)
#define TIMER0CNT_IRQ_ENABLE                (TIMER0CNT |= 0x0040)
#define PA_EnableTimer0() {TIMER0CNT_TIMER_START; TIMER0CNT_IRQ_ENABLE;}

#define TIMER0CNT_TIMER_STOP                (TIMER0CNT &= 0xFF7F)
#define TIMER0CNT_IRQ_DISABLE               (TIMER0CNT &= 0xFFBF)
#define PA_DisableTimer0() {TIMER0CNT_TIMER_STOP; TIMER0CNT_IRQ_DISABLE;}

// TIMER 1
#define TIMER1COUNT		*(volatile u16 *)(0x04000104)
#define TIMER1CNT		*(volatile u16 *)(0x04000106)
#define TIMER1CNT_TIMER_START               (TIMER1CNT |= 0x0080)
#define TIMER1CNT_IRQ_ENABLE                (TIMER1CNT |= 0x0040)
#define PA_EnableTimer1() {TIMER1CNT_TIMER_START; TIMER1CNT_IRQ_ENABLE;}

#define TIMER1CNT_TIMER_STOP                (TIMER1CNT &= 0xFF7F)
#define TIMER1CNT_IRQ_DISABLE               (TIMER1CNT &= 0xFFBF)
#define PA_DisableTimer1() {TIMER1CNT_TIMER_STOP; TIMER1CNT_IRQ_DISABLE;}

// TIMER 2
#define TIMER2COUNT		*(volatile u16 *)(0x04000108)
#define TIMER2CNT		*(volatile u16 *)(0x0400010A)
#define TIMER2CNT_TIMER_START               (TIMER2CNT |= 0x0080)
#define TIMER2CNT_IRQ_ENABLE                (TIMER2CNT |= 0x0040)
#define PA_EnableTimer2() {TIMER2CNT_TIMER_START; TIMER2CNT_IRQ_ENABLE;}

#define TIMER2CNT_TIMER_STOP                (TIMER2CNT &= 0xFF7F)
#define TIMER2CNT_IRQ_DISABLE               (TIMER2CNT &= 0xFFBF)
#define PA_DisableTimer2() {TIMER2CNT_TIMER_STOP; TIMER2CNT_IRQ_DISABLE;}

// TIMER 3
#define TIMER3COUNT		*(volatile u16 *)(0x0400010C)
#define TIMER3CNT		*(volatile u16 *)(0x0400010E)
#define TIMER3CNT_TIMER_START               (TIMER3CNT |= 0x0080)
#define TIMER3CNT_IRQ_ENABLE                (TIMER3CNT |= 0x0040)
#define PA_EnableTimer3() {TIMER3CNT_TIMER_START; TIMER3CNT_IRQ_ENABLE;}

#define TIMER3CNT_TIMER_STOP                (TIMER3CNT &= 0xFF7F)
#define TIMER3CNT_IRQ_DISABLE               (TIMER3CNT &= 0xFFBF)
#define PA_DisableTimer3() {TIMER3CNT_TIMER_STOP; TIMER3CNT_IRQ_DISABLE;}

*/


//extern funcpointer interruptfunc[14];


void interruptfunction(void);

/** @defgroup Interrupts Interrupt system
 *  Enable VBL, HBL, etc...
 *  @{
 */


/*! \fn void PA_vblFunc(void)
    \brief
         \~english The standard PAlib VBL function... This will update the pad, the stylus, the RTC, etc... You could/should use this function if you do your own custom VBL...
         \~french Fonction standard du VBL PAlib... Ceci met à jour le pad, le stylet, le RTC... Utilisez cette fonction si vous faites votre propre VBL...
*/
void PA_vblFunc(void);


/*! \def PA_InitVBL()
    \brief
         \~english Intiate a basic VBL function. Very usefull for unexperienced users, it updates the OAM (sprites), the Keypad, and the Stylus input every frame. No need to use PA_UpdateOAM, PA_UpdatePad, PA_UpdateStylus, and PA_UpdateRTC !
         \~french Initialise une fonction VBL basique. Très pratique pour l'utilisateur non averti, car met à jour l'OAM (les sprites), les touches, et le stylet... On n'a donc plus besoin d'utiliser PA_UpdateOAM, PA_UpdatePad,  PA_UpdateStylus, et PA_UpdateRTC dans ce cas !

*/


extern inline void PA_InitVBL(void) {
	//irqInit();
	irqSet(IRQ_VBLANK, PA_vblFunc);
	irqEnable(IRQ_VBLANK);
}

//PA_StartInt(INT_VBLANK, PA_vblFunc)


/*! \fn void PA_ResetInterrupts(void)
    \brief
         \~english Reset the interrupt system. Disables all interrupts.
         \~french Remise à 0 du système d'interruption. Désactive toutes les interruptions

*/
//void PA_ResetInterrupts(void);

/*! \fn void PA_StartInt(u8 inter, funcpointer interfunc)
    \brief
         \~english Initiate a given interrupt.
         \~french Initialise une interruption donnée
    \param inter
         \~english Interrupt to initiate. Use the following macros to chose : INT_VBLANK, INT_HBLANK, INT_VCOUNT,  INT_TIMER0, INT_TIMER1, INT_TIMER2, INT_TIMER3, INT_COM, INT_DMA0, INT_DMA1, INT_DMA2, INT_DMA3, INT_KEY, INT_CART...
         \~french Initialise une interruption donnée. Utiliser les macros suivantes : INT_VBLANK, INT_HBLANK, INT_VCOUNT,  INT_TIMER0, INT_TIMER1, INT_TIMER2, INT_TIMER3, INT_COM, INT_DMA0, INT_DMA1, INT_DMA2, INT_DMA3, INT_KEY, INT_CART...
    \param interfunc
         \~english Function to be executed at each interruptiono
         \~french Fonction devant etre executée à chaque interruption


*/
//void PA_StartInt(u8 inter, funcpointer interfunc);
/*extern inline void PA_StartInt(u8 inter, funcpointer interfunc){
	irqSet(inter, interfunc);
	irqEnable(inter);
}*/

/*! \fn void PA_StopInt(u8 inter)
    \brief
         \~english Initiate a given interrupt.
         \~french Initialise une interruption donnée
    \param inter
         \~english Interrupt to STOP. Use the following macros to chose : INT_VBLANK, INT_HBLANK, INT_VCOUNT,  INT_TIMER0, INT_TIMER1, INT_TIMER2, INT_TIMER3, INT_COM, INT_DMA0, INT_DMA1, INT_DMA2, INT_DMA3, INT_KEY, INT_CART...
         \~french ARRETER une interruption donnée. Utiliser les macros suivantes : INT_VBLANK, INT_HBLANK, INT_VCOUNT,  INT_TIMER0, INT_TIMER1, INT_TIMER2, INT_TIMER3, INT_COM, INT_DMA0, INT_DMA1, INT_DMA2, INT_DMA3, INT_KEY, INT_CART...
*/
/*extern inline void PA_StopInt(u8 inter){
	irqDisable(inter);
}*/


/*! \def PA_GetVcount()
    \brief
         \~english Get the vertical line count...
         \~french Récupérer le numéro de ligne tracée
*/
#define PA_GetVcount() (REG_VCOUNT&511)

/*! \fn void PA_ResetVBLCounters(void)
    \brief
         \~english Resets the VBL counters
         \~french Remet à 0 les compteurs VBL
*/
void PA_VBLCountersReset(void);

/*! \fn extern inline void PA_VBLCounterStart(u8 nCounter)
    \brief
         \~english Resets a given counter and starts running
         \~french Remet à 0 un compteur donné et le démarre
    \param nCounter
         \~english Counter number (0-15)
         \~french Numéro du compteur (0-15)
*/
extern inline void PA_VBLCounterStart(u8 nCounter){
	PA_VBLCounter[nCounter] = 0;
	PA_VBLCounterOn[nCounter] = 1;
}

/*! \fn extern inline void PA_VBLCounterPause(u8 nCounter)
    \brief
         \~english Pauses a given VBL counter
         \~french Met en pause un compteur VBL donné
    \param nCounter
         \~english Counter number (0-15)
         \~french Numéro du compteur (0-15)
*/
extern inline void PA_VBLCounterPause(u8 nCounter){
	PA_VBLCounterOn[nCounter] = 0;
}


/*! \fn extern inline void PA_VBLCounterUnpause(u8 nCounter)
    \brief
         \~english Unpauses a given VBL counter
         \~french Refait tourner un compteur VBL donné
    \param nCounter
         \~english Counter number (0-15)
         \~french Numéro du compteur (0-15)
*/
extern inline void PA_VBLCounterUnpause(u8 nCounter){
	PA_VBLCounterOn[nCounter] = 1;
}


extern inline void PA_VBLFunctionInit(funcpointer VBLFunc){
	CustomVBL = VBLFunc;
}

extern inline void PA_VBLFunctionReset(void){
	PA_VBLFunctionInit(PA_Nothing);
}








/** @} */ // end of Interrupts

#ifdef __cplusplus
}
#endif
#endif


