#ifndef _PA_FS
#define _PA_FS

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_FS.h
    \brief PA File System (obsolete)

    PA File System; don't use this anymore! Use libfat and / or EFS_Lib instead!
*/
#include "PA_Text.h"


#define PA_DS_CART  0x02000000
#define PA_GBA_CART 0x08000000

typedef struct{
	char Name[64];
	char Ext[6];
	u32 Length;
	u32 FilePos;
}PAFSFiles;

typedef struct{
	char Name[64];
	u32 FirstFile;  // Number of the first file...
	u32 NFiles;    // Number of files
	u32 FirstFolder;  // Number of the first folder...
	u32 NFolders;    // Number of folders	
}PAFSFolders;


typedef struct{
	u32 Nfolders; // Nombre de fichiers
	u32 Nfiles; // Nombre de dossiers
} PAFSSystems;

extern PAFSSystems *PA_FSSys;
extern PAFSFiles *PA_FSFile; // Pointeur sur les infos fichiers...
extern PAFSFolders *PA_FSFolder; 

extern char *PAFSStart;

// PAFS Start if in Ram...
#define PA_FSRam(size) char PA_FileSystem[size] = "PAFS007"; 


void PA_FSAllInit(void);// Inits for all systems



/** @defgroup PAFS PA File System (obsolete)
 *  PA File System; don't use this anymore! Use libfat and / or EFS_Lib instead!
 *  @{
 */

/*! \fn extern inline u32 PA_FSInit(void)
    \brief
         \~english Initialises PAFS and returns the number of files found... Stores the file infos in the following form : PA_FSFile[file number].Name is the file name without the extension, .Ext si the file extension (txt, html, etc...), and .Length is the file length. PA_PAFSFile(FileNumber) is a pointer towards a given file
         \~french Initialise PAFS et renvoie le nombre de fichiers trouvés... Stock e les infos sous la forme suivante : PA_FSFile[numéro du fichier].Name est son nom sans extension, .Ext est l'extension (txt, html, etc...), et .Length la taille du fichier. PA_PAFSFile(FileNumber) est un pointeur vers le fichier
*/
extern inline u32 PA_FSInit(void)
{
	REG_EXMEMCNT &= ~0x80; // GBA cart access
	PA_FSAllInit();
	return (PA_FSSys->Nfiles);
}



/*! \fn extern inline u32 PA_FSRamInit(void)
    \brief
         \~english Initialises PAFS and returns the number of files found... Stores the file infos in the following form : PA_FSFile[file number].Name is the file name without the extension, .Ext si the file extension (txt, html, etc...), and .Length is the file length. PA_PAFSFile(FileNumber) is a pointer towards a given file. This version stores the files in the ram, which makes it work in emulators and on WMB, but is limited to a few megs...
         \~french Initialise PAFS et renvoie le nombre de fichiers trouvés... Stock e les infos sous la forme suivante : PA_FSFile[numéro du fichier].Name est son nom sans extension, .Ext est l'extension (txt, html, etc...), et .Length la taille du fichier. PA_PAFSFile(FileNumber) est un pointeur vers le fichier. Cette version cope les fichiers en ram au lieu de la rom, et marche donc pour les emulateurs et sur WMB, mais est limité à quelques Mo du coup...
*/
extern inline u32 PA_FSRamInit(void)
{
	PAFSStart-=8;
	
	while (!((PAFSStart[0] == 'P') && (PAFSStart[1] == 'A') && (PAFSStart[2] == 'F') && (PAFSStart[3] == 'S') && (PAFSStart[4] == '0') && (PAFSStart[5] == '0'))) PAFSStart++;
	
	PAFSStart += 8;
	PA_FSSys = (PAFSSystems*)PAFSStart;

	while (!((PAFSStart[0] == 'D') && (PAFSStart[1] == 'I') && (PAFSStart[2] == 'R') && (PAFSStart[3] == '0') && (PAFSStart[4] == '0') && (PAFSStart[5] == '0'))) PAFSStart++;
	
	PAFSStart += 8;
	PA_FSFolder = (PAFSFolders*)PAFSStart;
	
	while (!((PAFSStart[0] == 'F') && (PAFSStart[1] == 'I') && (PAFSStart[2] == 'L') && (PAFSStart[3] == '0') && (PAFSStart[4] == '0') && (PAFSStart[5] == '0'))) PAFSStart++;
	
	PAFSStart+=8;
	PA_FSFile = (PAFSFiles*)PAFSStart;
	
	while (!((PAFSStart[0] == 'B') && (PAFSStart[1] == 'I') && (PAFSStart[2] == 'N') && (PAFSStart[3] == '0') && (PAFSStart[4] == '0') && (PAFSStart[5] == '0'))) PAFSStart++;

	PAFSStart+=8;
	return (PA_FSSys->Nfiles);
}


/*! \def PA_PAFSFile(FileN)
    \brief
         \~english Returns a pointer towards the given file number
         \~french Renvoie un pointeur vers le fichier donné...
*/
#define PA_PAFSFile(FileN) ((void*)(PAFSStart+PA_FSFile[FileN].FilePos))



/*! \fn extern inline s32 PA_FSGetFolder(char *name)
    \brief
         \~english Search in PAFS for a given folder. Returns the number of the folder in the PA File System, or -1 if no folder was found
         \~french Chercher un dossier donné dans PAFS. Renvoie le numéro du dossier dans le systeme de fichier de PA, ou -1 si le dossier n'a pas été trouvé
    \~\param name
         \~english Folder name...
         \~french Nom du dossier...
*/
extern inline s32 PA_FSGetFolder(char *name){
u16 i;

for (i = 0; i < PA_FSSys->Nfolders; i++){
	if (PA_CompareText(PA_FSFolder[i].Name, name)) 
		return i;
}

return -1;
}


/*! \fn extern inline s32 PA_FSGetFile(s16 start, char *name, char *extension)
    \brief
         \~english Search in PAFS for a given file. Returns the number of the file in the PA File System, or -1 if no file was found
         \~french Chercher un fichier donné dans PAFS. Renvoie le numéro du fichier dans le systeme de fichier de PA, ou -1 si le fichier n'a pas été trouvé
    \~\param start
         \~english From which file to start the search, usualy 0... Can be useful if you are searching all the files with a given extension
         \~french A partir de quel fichier commencer la recherche, en général 0... Peut être utile si on cherche tous les fichiers avec une extension donnée
    \~\param name
         \~english File name, without the extension. Putting "" as a file name should search all the files with the given extension
         \~french Nom du fichier, sans extension. Mettre "" comme nom devrait chercher chaque fichier avec l'extension donnée
    \~\param extension
      \~english File extension (ex : txt). Putting "" should search all the files with the given name
      \~french Extension du fichier (ex : txt). Mettre "" devrait chercher chaque fichier avec le nom donné
*/
extern inline s32 PA_FSGetFile(s16 start, char *name, char *extension){
u16 i;

for (i = start; i < PA_FSSys->Nfiles; i++){
	if (PA_CompareText(PA_FSFile[i].Name, name)&& PA_CompareText(PA_FSFile[i].Ext, extension)) 
		return i;
}

return -1;
}


/*! \fn s16 PA_FSSearchExt(s16 *array, char *extension)
    \brief
         \~english Puts the number of all the PAFS files with a given extension in the array, and returns the number of files found
         \~french Cherche tous les fichiers de PAFS avec l'extension trouvée, place le numéro dans le tableau, et renvoie le nombre de fichiers trouvés...
    \~\param array
         \~english Array in which to store the PAFS file positions
         \~french Tableau dans lequel stocker la position des fichiers PAFS trouvés
    \~\param extension
         \~english Extension to search, such as "txt" 
         \~french Extension à chercher, comme "txt"
*/
s16 PA_FSSearchExt(s16 *array, char *extension);



/*! \fn extern inline void *PA_Malloc(void *pointer, u32 size)
    \brief
         \~english Safe malloc, checks and frees the memory if necessary first
         \~french Malloc sûr, vérifie et libère la mémoire avant si nécessaire
    \~\param pointer
         \~english Pointer...
         \~french Pointer...
    \~\param size
         \~english Malloc size
         \~french Taille du malloc
*/
extern inline void *PA_Malloc(void *pointer, u32 size){  
	if (pointer) {
		free(pointer);
	}

	return (void*)malloc(size) ;
}



/*! \fn void PA_FSBgLoad(u8 screen, u8 bg_number, u32 filenumber)
    \brief
         \~english Easiest way to load a background converted with PAGfx... from PAFS !
         \~french Moyen le plus simple de charger un fond créé avec PAGfx... depuis PAFS !
    \~\param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_number
         \~english Background number... (0-3)
         \~french Numéro du fond...	 (0-3)
    \~\param filenumber
         \~english backgroundname_Info's file number in PAFS
         \~french Numéro du fichier nomdufond_Info dans PAFS
*/
extern inline void PA_FSBgLoad(u8 screen, u8 bg_number, u32 filenumber)  {  
	PA_EasyBgLoadEx(screen, bg_number, (u32*)PA_PAFSFile(filenumber), PA_PAFSFile(filenumber+3), PA_FSFile[filenumber+3].Length, PA_PAFSFile(filenumber+1), PA_FSFile[filenumber+1].Length, (void*)(PA_PAFSFile(filenumber+2)));
}



/*! \fn void PA_FSBgNameLoad(u8 screen, u8 bg_number, char* bg_name)
    \brief
         \~english Load a background from PAFS using its name...
         \~french Charger un fond depuis PAFS en utilisant son nom...
    \~\param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'écran (0 ou 1)
    \~\param bg_number
         \~english Background number... (0-3)
         \~french Numéro du fond...	 (0-3)
    \~\param bg_name
         \~english Background name
         \~french Nom du fond
*/
void PA_FSBgNameLoad(u8 screen, u8 bg_number, char* bg_name);







/** @} */ // end of PAFS


#ifdef __cplusplus
}
#endif
#endif


