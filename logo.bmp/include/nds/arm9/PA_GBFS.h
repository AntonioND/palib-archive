#ifndef _PA_GBFS
#define _PA_GBFS

#ifdef __cplusplus
extern "C" {
#endif

/*! \file PA_GBFS.h
    \brief Easy GBFS wrapper

    GBFS wrapper
*/


#include "PA_Text.h"
#include "gbfs.h"

extern inline u8 PA_CompareText(char *text1, char *text2){
	u8 ok = 1; // Devrait etre bon
	u8 i = 0; 
	while(ok && text2[i]){
		if (text1[i] != text2[i]) ok = 0;
		i++;
	}
	return ok;
}


typedef struct{
	void *File; // Position du fichier...
	char Name[30]; // Nom du fichier, sans extension
	char Ext[6]; // Extension du fichier !
	u32 Length; // Longueur
} PA_GBFS_infos;
extern PA_GBFS_infos PA_GBFSfile[200]; 
extern s16 PA_GBFS_nfiles;
extern GBFS_FILE const* PA_GBFS_FILE;

/** @defgroup GBFS GBFS wrapper
 *  Easily use GBFS...
 *  @{
 */

/*! \fn u32 PA_InitGBFS(void)
    \brief
         \~english Initialises GBFS and collects info on the files. Returns the number of files found... Stores the file infos in the following form : PA_GBFSfile[file number].File is a pointer towards the file, .Name is the file name without the extension, .Ext si the file extension (txt, html, etc...), and .Length is the file length.
         \~french Initialise GBFS et r�cup�re des infos sur les fichiers. Renvoie le nombre de fichiers trouv�s... Stock e les infos sous la forme suivante : PA_GBFSfile[num�ro du fichier].File est un pointeur sur le fichier en question, .Name est son nom sans extension, .Ext est l'extension (txt, html, etc...), et .Length la taille du fichier
*/
u32 PA_InitGBFS(void);



/*! \fn extern inline s32 PA_GetGBFSFile(s16 start, char *name, char *extension)
    \brief
         \~english Search in GBFS for a given file. Returns the number of the file in the PA GBFS system, or -1 if no file was found
         \~french Chercher un fichier donn� dans GBFS. Renvoie le num�ro du fichier dans le systeme PA GBFS, ou -1 si le fichier n'a pas �t� trouv�
    \~\param start
         \~english From which file to start the search, usualy 0... Can be useful if you are searching all the files with a given extension
         \~french A partir de quel fichier commencer la recherche, en g�n�ral 0... Peut �tre utile si on cherche tous les fichiers avec une extension donn�e
    \~\param name
         \~english File name, without the extension. Putting "" as a file name should search all the files with the given extension
         \~french Nom du fichier, sans extension. Mettre "" comme nom devrait chercher chaque fichier avec l'extension donn�e
    \~\param extension
      \~english File extension (ex : txt). Putting "" should search all the files with the given name
      \~french Extension du fichier (ex : txt). Mettre "" devrait chercher chaque fichier avec le nom donn�
*/
extern inline s32 PA_GetGBFSFile(s16 start, char *name, char *extension){
s16 i;

for (i = start; i < PA_GBFS_nfiles; i++){
	if (PA_CompareText(PA_GBFSfile[i].Name, name)&& PA_CompareText(PA_GBFSfile[i].Ext, extension)) 
		return i;
}

return -1;
}


/*! \fn s16 PA_GBFSSearchExt(s16 *array, char *extension)
    \brief
         \~english Puts the number of all the GBFS files with a given extension in the array, and returns the number of files found
         \~french Cherche tous les fichiers de GBFS avec l'extension trouv�e, place le num�ro dans le tableau, et renvoie le nombre de fichiers trouv�s...
    \~\param array
         \~english Array in which to store the GBFS file positions
         \~french Tableau dans lequel stocker la position des fichiers GBFS trouv�s
    \~\param extension
         \~english Extension to search, such as "txt" 
         \~french Extension � chercher, comme "txt"
*/
s16 PA_GBFSSearchExt(s16 *array, char *extension);


/** @} */ // end of GBFS


#ifdef __cplusplus
}
#endif

#endif


