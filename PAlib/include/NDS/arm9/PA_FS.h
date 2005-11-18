#ifndef _PA_FS
#define _PA_FS


/*! \file PA_FS.h
    \brief PA File System

    PA File System
*/
#include "PA_Text.h"

typedef struct{
	char Name[20];
	char Ext[6];
	u32 Length;
	u32 FilePos;
}PAFSFiles;

typedef struct{
	char Name[20];
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





/** @defgroup PAFS PA File System
 *  PA File System
 *  @{
 */

/*! \fn extern inline void PA_FSInit(void)
    \brief
         \~english Initialises PAFS and returns the number of files found... Stores the file infos in the following form : PA_FSFile[file number].Name is the file name without the extension, .Ext si the file extension (txt, html, etc...), and .Length is the file length. PA_PAFSFile(FileNumber) is a pointer towards a given file
         \~french Initialise PAFS et renvoie le nombre de fichiers trouv�s... Stock e les infos sous la forme suivante : PA_FSFile[num�ro du fichier].Name est son nom sans extension, .Ext est l'extension (txt, html, etc...), et .Length la taille du fichier. PA_PAFSFile(FileNumber) est un pointeur vers le fichier
*/
u32 PA_FSInit(void);

/*! \def PA_PAFSFile(FileN)
    \brief
         \~english Returns a pointer towards the given file number
         \~french Renvoie un pointeur vers le fichier donn�...
*/
#define PA_PAFSFile(FileN) ((void*)(PAFSStart+PA_FSFile[FileN].FilePos))




/*! \fn extern inline s32 PA_FSGetFile(s16 start, char *name, char *extension)
    \brief
         \~english Search in PAFS for a given file. Returns the number of the file in the PA File System, or -1 if no file was found
         \~french Chercher un fichier donn� dans PAFS. Renvoie le num�ro du fichier dans le systeme de fichier de PA, ou -1 si le fichier n'a pas �t� trouv�
    \param start
         \~english From which file to start the search, usualy 0... Can be useful if you are searching all the files with a given extension
         \~french A partir de quel fichier commencer la recherche, en g�n�ral 0... Peut �tre utile si on cherche tous les fichiers avec une extension donn�e
    \param name
         \~english File name, without the extension. Putting "" as a file name should search all the files with the given extension
         \~french Nom du fichier, sans extension. Mettre "" comme nom devrait chercher chaque fichier avec l'extension donn�e
    \param extension
      \~english File extension (ex : txt). Putting "" should search all the files with the given name
      \~french Extension du fichier (ex : txt). Mettre "" devrait chercher chaque fichier avec le nom donn�
*/
extern inline s32 PA_FSGetFile(s16 start, char *name, char *extension){
s16 i;

for (i = start; i < PA_GBFS_nfiles; i++){
	if (PA_CompareText(PA_FSFile[i].Name, name)&& PA_CompareText(PA_FSFile[i].Ext, extension)) 
		return i;
}

return -1;
}


/*! \fn s16 PA_FSSearchExt(s16 *array, char *extension)
    \brief
         \~english Puts the number of all the PAFS files with a given extension in the array, and returns the number of files found
         \~french Cherche tous les fichiers de PAFS avec l'extension trouv�e, place le num�ro dans le tableau, et renvoie le nombre de fichiers trouv�s...
    \param array
         \~english Array in which to store the GBFS file positions
         \~french Tableau dans lequel stocker la position des fichiers GBFS trouv�s
    \param extension
         \~english Extension to search, such as "txt" 
         \~french Extension � chercher, comme "txt"
*/
s16 PA_FSSearchExt(s16 *array, char *extension);


/** @} */ // end of PAFS



#endif


