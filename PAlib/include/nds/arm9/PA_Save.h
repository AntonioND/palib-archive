#ifndef _PA_Save
#define _PA_Save




  #define REG_SRAM 0x0A000000
  
/** @defgroup Saves Saving commands !!!
 *  With these functions you can save data to SRAM :-)
 *  @{
 */


/*! \def PA_Save8bit(offset, value)
    \brief
         \~english Save an 8 bit value to a given SRAM offset. Uses 1 offset
         \~french Sauvegarde une valeur 8 bits dans une section donnée de la SRAM. Utilise 1 section
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
    \param value
         \~english Value...
         \~french Valeur à enregistrer...
*/
#define PA_Save8bit(offset, value) SRAM[offset] = value

/*! \def PA_Save16bit(offset, value)
    \brief
         \~english Save a 16 bit value to a given SRAM offset. Uses 2 offsets...
         \~french Sauvegarde une valeur 8 bits dans une section donnée de la SRAM. Utilise 2 sections
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
    \param value
         \~english Value...
         \~french Valeur à enregistrer...
*/

#define PA_Save16bit(offset, value) {PA_Save8bit(offset, value&255); PA_Save8bit(offset + 1, value >> 8);}

/*! \def PA_Save32bit(offset, value)
    \brief
         \~english Save a 32 bit value to a given SRAM offset. Uses 4 offsets...
         \~french Sauvegarde une valeur 32 bits dans une section donnée de la SRAM. Utilise 4 sections
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
    \param value
         \~english Value...
         \~french Valeur à enregistrer...
*/

#define PA_Save32bit(offset, value) {PA_Save8bit(offset, value&255); PA_Save8bit(offset + 1, value >> 8); PA_Save8bit(offset + 2, value >> 16); PA_Save8bit(offset + 3, value >> 24);}


/*! \fn extern inline s16 PA_SaveString(s32 offset, char *string)
    \brief
         \~english Save a string (a name for example) to a given SRAM offset. The last value in the string must be 0. Offsets  used correspond to string lenght + 1. Returns the length
         \~french Sauvegarde une chaine de caractères (un nom par exemple) a u n endroit donné en SRAM. La dernière valeur doit etre 0. Utilise autant de place en SRAM que la longueur de la chaine + 1. Renvoie la longueur
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
    \param string
         \~english Character string...
         \~french Chaine de caractères...
*/

extern inline s16 PA_SaveString(s32 offset, char *string) {
u16 i = 0;

	while(string[i]) {
	SRAM[i + offset] = string[i];
	i++;
	}
   SRAM[i + offset] = string[i];
   return (i+1);
}


/*! \fn extern inline void PA_SaveData(s32 offset, void* Data, u32 NBytes)
    \brief
         \~english Save N Bytes in SRAM. This function can be used to backup a whole array.
         \~french Sauvegarde N Octets en SRAM. Cette fonction peut être utilisée pour sauvegarder un tableau complet...
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
    \param Data
         \~english Data to save
         \~french Données à sauvegarder
    \param NBytes
         \~english Number of Bytes to save
         \~french Nombre d'octets à sauvegarder
*/
extern inline void PA_SaveData(s32 offset, void* Data, u32 NBytes) {
u16 i = 0;
u8 *adress = (u8*)Data;
	for (i = 0; i < NBytes; i++) SRAM[i + offset] = adress[i];
}



/*! \def PA_Load8bit(offset)
    \brief
         \~english Returns a saved 8 bit value from a given SRAM offset.
         \~french Retourne une valeur 8 bits sauvegardée dans une section donnée de la SRAM. 
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
*/

#define PA_Load8bit(offset) SRAM[offset]

/*! \def PA_Load16bit(offset)
    \brief
         \~english Returns a saved 16 bit value from a given SRAM offset.
         \~french Retourne une valeur 16 bits sauvegardée dans une section donnée de la SRAM.
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
*/
#define PA_Load16bit(offset) (SRAM[offset] + (SRAM[offset+1] << 8))

/*! \def PA_Load32bit(offset)
    \brief
         \~english Returns a saved 32 bit value from a given SRAM offset.
         \~french Retourne une valeur 32 bits sauvegardée dans une section donnée de la SRAM.
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
*/
#define PA_Load32bit(offset) (PA_Load8bit(offset) + (PA_Load8bit(offset + 1) << 8) + (PA_Load8bit(offset + 2) << 16) + (PA_Load8bit(offset + 3) << 24))


/*! \fn extern inline s16 PA_LoadString(s32 offset, char *string)
    \brief
         \~english Returns a saved string from a given SRAM offset.Returns the string length
         \~french Retourne une chaine de caractères sauvegardée dans une section donnée de la SRAM. Renvoie la longueur de la chaine
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
    \param string
         \~english String in which to copy the saved string
         \~french Chaine dans laquelle copier la chaine en mémoire
*/
extern inline s16 PA_LoadString(s32 offset, char *string) {
s16 i = -1;

	do {
		i++;	
		string[i] = SRAM[i + offset];
	}
	while(string[i]);
	return i;
}


/*! \fn extern inline void PA_LoadData(s32 offset, void* Data, u16 NBytes)
    \brief
         \~english Returns the data saved using PA_SaveData. 
         \~french Renvoie les données sauvegardées avec PA_SaveData
    \param offset
         \~english Offset, from 0 to alot (depending on how much SRAM you use...)
         \~french Section dans la SRAM, de 0 à beaucoup (dépend de combien on utlise de SRAM)
    \param Data
         \~english Data to save
         \~french Données à sauvegarder
    \param NBytes
         \~english Number of Bytes to save
         \~french Nombre d'octets à sauvegarder
*/
extern inline void PA_LoadData(s32 offset, void* Data, u16 NBytes) {
u16 i = 0;
u8 *adress = (u8*)Data;
for (i = 0; i < NBytes; i++) adress[i] = SRAM[i + offset];
}

/** @} */ // end of Saves





#endif


