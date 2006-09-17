// Les Includes
#include <PA9.h>

//Fonction principale du code
int main(void)
{
//Initialision de PAlib
PA_Init();
PA_InitVBL();

PA_InitText(1,2);

while(1)
{
PA_OutputSimpleText(1,0,0,"                                                                      ");
if(Pad.Held.A)PA_OutputTextSpecial0(1,0,1,"Hello World !");
else if(Pad.Held.B)PA_OutputTextSpecial1(1,0,1,"Hello World !");
else if(Pad.Held.Start)PA_OutputTextSpecial2(1,0,1,"Hello World !");
else if(Pad.Held.R)PA_OutputTextSpecial3(1,0,1,"Hello World !");
else if(Pad.Held.L)PA_OutputTextSpecial4(1,0,1,"Hello World !");
else if(Pad.Held.Select)PA_OutputTextSpecial5(1,0,1,"Hello World !");
//La boucle infinie du programme
}
return 0;
}
