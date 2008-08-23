// ----- Change log -----
// 10/08/04 - Modified mixer to take any number of samples to mix, 
//            rather than strictly a full buffer, added MOD updating 
//            functions, and SndUpdate to mix sound and update MOD as 
//            needed for a full frame
// 10/21/04 - Moved MOD_NO_NOTE/MOD_NO_SAMPLE defines here, 
//            since nothing outside player needs to know them
// 05/05/05 - Converted to use periods instead of Hz.
//			  MOD timing is done fixed-point, mixer moved to IWRAM.
//			  Added SFX functions.
// 05/06/05 - Added all effect functions.
// 05/08/05 - Ported to DS.
// ----------------------

#include <nds.h>
#include <string.h>
#include <nds/arm7/audio.h>
#include <PA7.h>

#include "Sound7.h"
#include "SoundCommon.h"

// --- Sound functions ---

	// Call this once at startup
void SndInit7()
{
	u16 oldIME;

	oldIME = REG_IME;
	REG_IME = 0;

	TIMER0_CR = 0;
	REG_IE |= IRQ_TIMER0;

	REG_IME = oldIME;

}	// SndInit
