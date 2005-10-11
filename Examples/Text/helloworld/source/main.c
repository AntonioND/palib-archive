//copyright smealum http://jvteamds.free.fr
// Les Includes
#include <PA9.h>

//Fonction principale du code

int main(void)
{
//Initialision de PAlib
PA_Init();
PA_InitVBL();

PA_InitText(1,2);
PA_OutputSimpleText(1,1,1,"Hello World !");

while(1)
{
//La boucle infinie du programme
}
return 0;
}
