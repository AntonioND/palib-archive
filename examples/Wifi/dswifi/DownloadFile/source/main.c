#include <PA9.h>

#include "ky_geturl.h"

int main(void)
{
	const char url[] = "art.kh.nu/ds/r.php";
	int err, i;
	sBuffer bfr;
	
	defaultExceptionHandler();
	
	PA_Init();
	PA_Init16bitBg(1, 3);
	PA_LoadDefaultText(0, 2);
	PA_InitWifi();
	PA_ConnectWifiWFC();
	
	ky_InitBuffer(&bfr);
	for (;;) {
		PA_OutputText(0, 0, 0, "downloading image, please wait...");
		err = ky_GetUrl(url, &bfr);
		
		PA_OutputText(0, 0, 0, "                                                             ");
		PA_OutputText(0, 1, 1, "                                                                                       ");
		PA_Clear16bitBg(1);
		
		if (err < 0) {
			PA_OutputText(0, 1, 1, "Error: %d, %d, %d | %d: %s", err, ky_errno, ky_h_errno, errno, strerror(errno));
		} else {
			PA_OutputText(0, 1, 1, "%s<%d> Recv %d bytes.", (err == 0) ? "Done" : "Fail", err, bfr.length);
			if (bfr.length > 0) {
				PA_LoadJpeg(1, (void*)bfr.buffer);
			}
		}
		ky_FreeBuffer(&bfr);
		
		for (i=0; i < 60; ++i) PA_WaitForVBL();
	}
	
	return 0;
}