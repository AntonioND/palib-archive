#include "PA9.h"
//#include <stdio.h>


//int atoi(char*);

void PA_on_irqvideo() 
{	
  if(REG_IF & IRQ_VBLANK) {
    VBLANK_INTR_WAIT_FLAGS |= IRQ_VBLANK;
    REG_IF |= IRQ_VBLANK;
  }
  else {
    // Ignore all other interrupts
    REG_IF = REG_IF;
  }
}


void PA_InitInterruptHandlervideo()
{
  REG_IME = 0;
  IRQ_HANDLER = PA_on_irqvideo;
  REG_IE = IRQ_VBLANK;
  REG_IF = ~0;
  DISP_SR = DISP_VBLANK_IRQ;
  REG_IME = 1;
}


int PA_maxfilevideo(char *nomdufichier){
int numimages;
char nomfichier[255];
strcpy(nomfichier,nomdufichier);
strcat(nomfichier,".txt");
  WAIT_CR &= ~0x80;
  GBFS_FILE const* gbfs_file = 
    find_first_gbfs_file((void*)0x08000000);
  char* image = (char*)gbfs_get_obj(gbfs_file, 
				      nomfichier, 
				      0);
  numimages = atoi(image);
  WAIT_CR |= 0x80;
return numimages;
}


void PA_Initvideo(){
powerON(POWER_ALL);
  videoSetMode(MODE_5_2D | DISPLAY_BG2_ACTIVE);
  vramSetBankA(VRAM_A_MAIN_BG_0x6000000);
  BG2_CR = BG_BMP16_256x256;
  videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
  vramSetBankC(VRAM_C_SUB_BG);
  SUB_BG0_CR = BG_MAP_BASE(31);
  BG_PALETTE_SUB[255] = RGB15(31,31,31);
  PA_InitInterruptHandlervideo();
}


void PA_video(int numimages, char *nomdufichier){
int current_image = 0, width = 0, height = 0;
  while(1) {
    swiWaitForVBlank();
  char nomfichier[255];
  strcpy(nomfichier,nomdufichier);
  char num[255];
  sprintf(num,"%d",current_image);
  if(current_image > 99)strcat(nomfichier,num);
  else if(current_image > 9){
  strcat(nomfichier,"0");
  strcat(nomfichier,num);
  }
  else {
  strcat(nomfichier,"00");
  strcat(nomfichier,num);
  }  
  strcat(nomfichier,".frm");
      current_image++;
      if(current_image >= numimages)
	current_image = 0;
      memset(BG_GFX, 0, 256*256*2);
      PA_jpeg_BltImage(nomfichier, BG_GFX, 256, 256);
      PA_jpeg_GetImageSize(nomfichier, &width, &height);
  }
}
