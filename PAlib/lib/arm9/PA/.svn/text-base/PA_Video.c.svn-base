#ifdef __cplusplus
extern "C" {
#endif



#include <PA9.h>

#include <stdlib.h>
#include <stdio.h>

int *vidsizes, vidpos=0, *tab;
s8 hvidfile=0;
u8 bufferjpg[256*192*3];
int deja0;
char *buffer2;



char *ligne(char *buffer, int start, int *end){
	int i;
	free(buffer2);
	buffer2 = (char*)malloc(sizeof(char)*10);
	memset(buffer2,0,sizeof(buffer2));
	
	for(i=start;buffer[i]!='\n';i++){
		int sup=0;
		if(buffer[i]>='0'&&buffer[i]<='9')buffer2[i-start-sup]=buffer[i];
		else sup++;
	}
	
	buffer2[i-2]='\0';
	if(buffer2[0]=='0'){
		*end=i+1;
		return "0";
	}
	//PA_OutputText(0, 0, 10, "%s            ",buffer2);
	//return buffer2;
	*end=i+1;
	return buffer2;
}



char *correctname(char *name1){
	char *name2;
	name2 = (char*)malloc(sizeof(char)*255);
	int i;
	for(i=0;name1[i]!='_';i++)name2[i]=name1[i];
	name2[i]='\0';
	return name2;
}



int jpgcorrect(u8* video,int num, int *size){
	int i;
	int final=size[num+1],depart=size[num];
	i=1;
	while(depart==1){
		depart = size[num+i];
		i++;
	}
	while(final==1){
		final = size[num+1+i];
		i++;
		}
		for(i=depart;i<final;i++){
			bufferjpg[i-depart]=video[i];
		}
		//PA_OutputText(0, 0, 0, "%d :\n%d -> %d       \n",num,depart,final);
		if(final==0)return 0;
		
		return 1;
	}
	
	
	int PA_LoadVid(u8* video,int *taille, int fps,int depart){
	int i=depart;
	
	
	while(taille[i]>0){
		if(taille[i]==1)PA_WaitForVBL();
		else PA_LoadJpeg(1, (void*)jpgcorrect(video,i,taille));	
		i++;
		//PA_OutputText(0, 0, 0, "num :  %d",i);
		int j;
		for(j=0;(float)j<(float)10*(16/fps);j++)PA_WaitForVBL();
	}
	return i;
}



int numligne(char *buffer){
	int i,count=0;
	for(i=0;i<(int)strlen(buffer);i++)if(buffer[i]=='\n')count++;
	//PA_OutputText(0, 0, 0, "numligne : %d",count);
	return count;
}



int remplacesize(int num){
	if(deja0==0&&vidpos<(int)strlen((char*)PA_PAFSFile(hvidfile))){
		int end;
		vidsizes[num]=atoi(ligne((char*)PA_PAFSFile(hvidfile),vidpos,&end));
		if(vidsizes[num]==0)deja0=1;
		vidpos=end;
		//PA_OutputText(0, 0, 23, "%d",vidsizes[num]);
		return 1;
	}
	return 0; 
}



int PA_FSLoadVid(s8 file,int *taille, int fps,int depart){
	tab = (int*)malloc(sizeof(int)*500);
	int a,j;
	for(a=0;a<500;a++)tab[a]=taille[a+depart];
	int i=0;
	//remplacesize(0);
	while(tab[i]>0){
		if(tab[i]==1)PA_WaitForVBL();
		else if(jpgcorrect((u8*)PA_PAFSFile(file),i,tab))PA_LoadJpeg(1, (void*)bufferjpg);	
		remplacesize(i);
		i++;
		//PA_OutputText(0, 0, 17, "num : %d :\n %d  \n %d   \n %d   %d ",i, tab[i],tab[i+1],tab[i+2]);
		for(j=0;(float)j<(float)7*(16/fps);j++)PA_WaitForVBL();
	}
	for(j=i;deja0==0;j++){
		remplacesize(j);
	}
	free(tab);
	return i;
}

void PA_LoadMultiVid(int *taille, int fps,int numvid, ...){
	int soustrait=0;
	 va_list varg; // Variable stockant la somme
	 va_start(varg, numvid); // On initialise la variable
	 
	 do {
		 u8* video = va_arg(varg, u8*); // On fais la somme	
		 soustrait=PA_LoadVid(video,taille,fps,soustrait+2);
		 numvid--;
	 }while(numvid!=0);
	 
	 va_end(varg); // On détruit la variable
	}
	int PA_FSVidfps(s8 file){
	int fps;
	char rien[255];
	sscanf((char*)PA_PAFSFile(file),"int %s = %d;",rien,&fps);
	return fps;
}




void PA_FSsizes(s8 file, int start1, int *end1){
	int temp;
	vidsizes = (int*)malloc(sizeof(int)*500);
	memset(vidsizes,0,sizeof(vidsizes));
	int i=0;
	int start=start1, end=0;
	ligne((char*)PA_PAFSFile(file),start,&end);
	//PA_OutputText(0, 0, 5, "ligne en cours : %d",i);
	start=end;
	//PA_OutputText(0, 0, 15, "start : %d         ",start);
	
	for(i=0;i<numligne((char*)PA_PAFSFile(file))-1;i++){
		temp=atoi(ligne((char*)PA_PAFSFile(file),start,&end));
		vidsizes[i]=temp;
		start=end;
		PA_OutputText(0, 0, 5, "                             \n                               ");
		if(i<100){
			//PA_OutputText(0, 25, i, "%d ",sizes[i]);
			//PA_OutputText(0, 15, i, "%d ",temp);
		}
		//PA_OutputText(0, 0, 5, "ligne en cours : %d :\n %d,\nsizes = %d = %d",i,start,temp,sizes[i]);
		if(temp==0&&i>2){
		//PA_OutputText(0, 0, 8, "ok");
		*end1=end;
		break;
		}
	}
}

int PA_FSLoadMultiVid(s8 file){
	hvidfile=file;
	int start = 0,numvid = 0;
	vidsizes = (int*)malloc(sizeof(int)*500);
	char name[255];
	int end=0;
	int fps = PA_FSVidfps(file);
	//PA_OutputText(0, 0, 5, "fps : %d",fps);
	//PA_OutputText(0, 0, 8, "    ");
	memset(vidsizes,0,sizeof(vidsizes));
	PA_FSsizes(file,start,&end);
	start=end;
	vidpos=end;
	
	while(1){
		deja0 = 0;
		sprintf(name,"%s%d",correctname(PA_FSFile[file].Name),numvid);
		//PA_OutputText(0, 0, 5, "name : %s      ",name);
		s8 vidfile;
		vidfile = PA_FSGetFile(0,name,"vid");
		//PA_LoadJpeg(0,PA_GBFSfile[vidfile].File);
		int a;
		for(a=0;a<100;a++)PA_WaitForVBL();
		if(vidfile>-1)PA_FSLoadVid(vidfile,vidsizes, fps,1);
		else return 1;
		numvid++;
		//PA_OutputText(1, 0, 5, "%d %d",end ,numvid);
	}
	return 1;
	//PA_LoadMultiVid(file, end, numvid+1);
}



#ifdef __cplusplus
}
#endif




