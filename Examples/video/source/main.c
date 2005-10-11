#include <PA9.h>

int main(void)
{
PA_Initvideo();
int numimages = PA_maxfilevideo("3dmoto");
PA_video(numimages,"3dmoto");
while(1){}
  return 0;
}
