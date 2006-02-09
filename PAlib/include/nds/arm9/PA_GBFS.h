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

/*! \fn extern inline void PA_InitGBFS(void)
    \brief
         \~english Initialises GBFS and collects info on the files. Returns the number of files found... Stores the file infos in the following form : PA_GBFSfile[file number].File is a pointer towards the file, .Name is the file name without the extension, .Ext si the file extension (txt, html, etc...), and .Length is the file length.
         \~french Initialise GBFS et récupère des infos sur les fichiers. Renvoie le nombre de fichiers trouvés... Stock e les infos sous la forme suivante : PA_GBFSfile[numéro du fichier].File est un pointeur sur le fichier en question, .Name est son nom sans extension, .Ext est l'extension (txt, html, etc...), et .Length la taille du fichier
*/
extern inline u32 PA_InitGBFS(void){
s16 dot = 0;
s16 i = 0;

	WAIT_CR &= ~0x80;
 
	PA_GBFS_FILE = find_first_gbfs_file((void*)0x08000000);
	PA_GBFS_nfiles = 0;
	PA_GBFSfile[PA_GBFS_nfiles].File = (void*)gbfs_get_nth_obj(PA_GBFS_FILE, PA_GBFS_nfiles, (char*)PA_GBFSfile[PA_GBFS_nfiles].Name, &PA_GBFSfile[PA_GBFS_nfiles].Length);

while(PA_GBFSfile[PA_GBFS_nfiles].File != NULL){

	// On cherche la fin du nom
	while(PA_GBFSfile[PA_GBFS_nfiles].Name[i]) i++;
	// On en déduit le dernier point, pour avoir l'extension
	while(PA_GBFSfile[PA_GBFS_nfiles].Name[i] != '.') i--; // On cherche l'extension
	dot = i+1;
	
	for (i = 0; PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot] ; i++){
		PA_GBFSfile[PA_GBFS_nfiles].Ext[i] = PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot];
		if (('A' <= PA_GBFSfile[PA_GBFS_nfiles].Ext[i])&&(PA_GBFSfile[PA_GBFS_nfiles].Ext[i] <= 'Z')) PA_GBFSfile[PA_GBFS_nfiles].Ext[i] += 'a' - 'A'; // On passe en minuscule
		PA_GBFSfile[PA_GBFS_nfiles].Ext[i+1] = 0;
		PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot-1] = 0;
		if (PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot-2] == '.') PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot-2] = 0;
	}
	PA_GBFSfile[PA_GBFS_nfiles].Name[dot] = 0;
	PA_GBFSfile[PA_GBFS_nfiles].Ext[dot] = 0;
	
	++PA_GBFS_nfiles; // On ajoute un objet...
	PA_GBFSfile[PA_GBFS_nfiles].File = (void*)gbfs_get_nth_obj(PA_GBFS_FILE, PA_GBFS_nfiles, (char*)PA_GBFSfile[PA_GBFS_nfiles].Name, &PA_GBFSfile[PA_GBFS_nfiles].Length);
}

return PA_GBFS_nfiles;
}



/*! \fn extern inline s32 PA_GetGBFSFile(s16 start, char *name, char *extension)
    \brief
         \~english Search in GBFS for a given file. Returns the number of the file in the PA GBFS system, or -1 if no file was found
         \~french Chercher un fichier donné dans GBFS. Renvoie le numéro du fichier dans le systeme PA GBFS, ou -1 si le fichier n'a pas été trouvé
    \param start
         \~english From which file to start the search, usualy 0... Can be useful if you are searching all the files with a given extension
         \~french A partir de quel fichier commencer la recherche, en général 0... Peut être utile si on cherche tous les fichiers avec une extension donnée
    \param name
         \~english File name, without the extension. Putting "" as a file name should search all the files with the given extension
         \~french Nom du fichier, sans extension. Mettre "" comme nom devrait chercher chaque fichier avec l'extension donnée
    \param extension
      \~english File extension (ex : txt). Putting "" should search all the files with the given name
      \~french Extension du fichier (ex : txt). Mettre "" devrait chercher chaque fichier avec le nom donné
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
         \~french Cherche tous les fichiers de GBFS avec l'extension trouvée, place le numéro dans le tableau, et renvoie le nombre de fichiers trouvés...
    \param array
         \~english Array in which to store the GBFS file positions
         \~french Tableau dans lequel stocker la position des fichiers GBFS trouvés
    \param extension
         \~english Extension to search, such as "txt" 
         \~french Extension à chercher, comme "txt"
*/
s16 PA_GBFSSearchExt(s16 *array, char *extension);


/** @} */ // end of GBFS


#ifdef __cplusplus
}
#endif

#endif


