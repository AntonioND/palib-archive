// Includes, only one sprite
#include <PA9.h>
#include <stdio.h>
// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

//my includes


#define levellength 128
#define levelheight 24

#define norm_fix(x)			((x)<<8)
#define fix_norm(x)			((x)>>8)  //xxx truncates instead of rounds
#define fix_mult(x,y)		(((x)*(y))>>8)
#define ratio_fix(x,y)	((256*(x))/(y))
#define fixed_fraction(x)			((x)&&0xff)


#define GRAVITY 48
#define MARIO_SPEED 512
#define maxcoins 50
#define maxenemies 20

void MoveMario(void);
void scroll();
void CheckCollisions(void);
void placecoins();
u8 GetTile(s16 x, s16 y);
u8 LeftCollision(void);
u8 RightCollision(void);
u8 DownCollision(void);
u8 TouchingGround(void);
u8 tilecollsion(s32,s32,u8,u8);
u8 allcollision();
u8 collision(s32,s32,u8,u8);
u8 getsprite();
void deletesprite(u8);


typedef struct{
   s8 x, y, flipx;
}pointinfo; 

typedef struct{
   pointinfo left, right, up, down;
   u8 flipped;
}hitboxinfo;

typedef struct {
   s32 x,y;
   u8 alive;
   u8 sprite;
} coininfo;

typedef struct{

	s32 x, y;
	s32 vy; // used for jumping...
	s32 scrollx, scrolly; // Scroll value...
	u8 sprite; //the sprite number
	u8 jumping;
	hitboxinfo hitbox;
}mariotype;


mariotype mario;
//pointer to items
//pointer to enemies
u8 sprite[128];

coininfo coin[maxcoins];

// Main function
int main(void)	{
	// PAlib init
	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1, 0);
	
	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Palette....	

	PA_EasyBgLoad(0, 1, mario_world); // platfroms...
	PA_EasyBgLoad(0, 2, hills); // hills
	PA_EasyBgLoad(0, 3, back); // back
	
	PA_InitParallaxX(0, 0, 256, 128, 64);
	
	
	mario.x = norm_fix(60); mario.y = norm_fix(128-32); // bottom of the screen... fixed point
	mario.vy = 0; // not jumping
	mario.hitbox.flipped=0;
	mario.scrollx = 0;
	mario.sprite=getsprite();
	
	mario.hitbox.left.x=2; mario.hitbox.left.y=8; mario.hitbox.right.x=29; mario.hitbox.right.y=8; mario.hitbox.down.x=10; mario.hitbox.down.y=31; mario.hitbox.up.x=5; mario.hitbox.up.y=0;

	mario.hitbox.left.flipx=0; mario.hitbox.right.flipx=0; mario.hitbox.up.flipx=10; mario.hitbox.down.flipx=11;
	
	PA_CreateSprite(0, mario.sprite,(void*)mario_Sprite, OBJ_SIZE_32X32,1, 0, fix_norm(mario.x), fix_norm(mario.y)); // Sprite
	

	 placecoins();
	while(1)
	{	
	  
		MoveMario();
		scroll();
		PA_WaitForVBL();
	}
	
	return 0;
}



void MoveMario(void){
	if(Pad.Newpress.Right) {
		PA_StartSpriteAnim(0, 0, 0, 1, 6);	
		PA_SetSpriteHflip(0, 0, 0);
		mario.hitbox.flipped = 0;
	}
	else if(Pad.Newpress.Left) {
		PA_StartSpriteAnim(0, 0, 0, 1, 6);	
		PA_SetSpriteHflip(0, 0, 1);
		mario.hitbox.flipped = 1;
	}
	
	if ((Pad.Newpress.A) && (TouchingGround())){  // If pressed A and not in the air
		mario.vy = -1500; // Start jumping
		mario.jumping=true;
	}
	
	if(mario.jumping && !Pad.Held.A && mario.vy<-400){
		mario.vy=-400;
	}		

	// Moving Code
	mario.x += (Pad.Held.Right - Pad.Held.Left)*MARIO_SPEED;	 // in fixed point...
	
	// Add gravity
	if(!TouchingGround()){
		mario.vy += GRAVITY;
	}		
		mario.y += mario.vy;
			
	
	CheckCollisions();
	
	if (!TouchingGround()) PA_SetSpriteAnim(0, 0, 2); // Not on the ground
	else if(!((Pad.Held.Left)||(Pad.Held.Right))) PA_SetSpriteAnim(0, 0, 0);// Image if not in the air and not walking
}

void scroll(){
	
	
	if (((fix_norm(mario.x-mario.scrollx)) > 160) && (fix_norm(mario.x) < 1024-128)){ // Scroll more...
		mario.scrollx = mario.x - norm_fix(160);		
	}
	else if ((((mario.x-mario.scrollx)>>8) < 64) && ((mario.x>>8) > 64)){
		mario.scrollx = mario.x - norm_fix(64);		
	}
	
	
	//automatic scrolling
//	mario.scrollx+=ratio_fix(1,2);
//	mario.scrolly+=ratio_fix(1,2);
		
	PA_ParallaxScrollXY(0, fix_norm(mario.scrollx),fix_norm(mario.scrolly));
	
	//move player
	PA_SetSpriteXY(0, mario.sprite, fix_norm(mario.x-mario.scrollx), fix_norm(mario.y-mario.scrolly));
	
	
	int i;
	for(i=0;i<maxcoins;i++){
	   if(coin[i].alive){
		   	if(coin[i].x<=fix_norm(mario.scrollx)+256 && coin[i].x>=fix_norm(mario.scrollx)-8){
			      //if it is then move it to the correct position
			      if(coin[i].sprite!=0){
			      	PA_SetSpriteXY(0,coin[i].sprite,coin[i].x-fix_norm(mario.scrollx),coin[i].y-fix_norm(mario.scrolly));
			    	}
			    	//if it's not then create a sprite for it
					else{
					   coin[i].sprite=getsprite();
						PA_CreateSprite(0, coin[i].sprite,(void*)coin_Sprite, OBJ_SIZE_8X8,1, 0, coin[i].x-fix_norm(mario.scrollx), coin[i].y-fix_norm(mario.scrolly)); 	
					}			
				}
			   //if the coin is offscren delete it
			   else{
			      //don't delete already gone stuff
			      if(coin[i].sprite!=0){
				      deletesprite(coin[i].sprite);
				      coin[i].sprite=0;
				   }   
			   }
			}
	}
						   
	//PA_OutputText(1, 2, 11, "X : %d   ", fix_norm(mario.x));
	//PA_OutputText(1, 2, 10, "Y : %d   ", mario.y);
	//PA_OutputText(1, 2, 12, "Scroll : %d   ", fix_norm(mario.scrollx));
}		

   
u8 GetTile(s16 x, s16 y){
	//if (y > fix_norm(mario.scrolly)+192) return 1; //collide with bottom
	if (y < fix_norm(mario.scrolly)) return 1; //collide with top
	return platformcollisionmap3_Map[((y>>3)*levellength) + (x>>3)];
}

//check for a collision with a tile at pointx, pointy, and adding in flipx if the sprite is flipped
u8 TileCollision(pointinfo point, u8 flipped){
   return (GetTile(fix_norm(mario.x)+point.x + (flipped*point.flipx), fix_norm(mario.y)+point.y));
}

//checks if mario is on the ground
u8 TouchingGround(void){
	return GetTile((fix_norm(mario.x))+10 + (mario.hitbox.flipped*11), (fix_norm(mario.y))+32);
}

//checks if any of the collisions are true for that tile and returns what side
u8 anycollision(u8 tile, hitboxinfo hitbox){
   if(TileCollision(hitbox.left, hitbox.flipped)==tile)return 1;
   if(TileCollision(hitbox.right, hitbox.flipped)==tile)return 2;
   if(TileCollision(hitbox.up, hitbox.flipped)==tile)return 3;
   if(TileCollision(hitbox.down, hitbox.flipped)==tile)return 4;
   else return 0;
}  
 
u8 collision(s32 x2, s32 y2, u8 h2, u8 w2){
   if (	(fix_norm(mario.x)>x2+w2) || (fix_norm(mario.x)+30<x2) ||
			(fix_norm(mario.y)>y2+h2) || (fix_norm(mario.y)+30<y2)){
      return 0;
   }
	else{
	   return 1;
	}		   
}
   
//checks if anything has collided with the player 
// tile 1 is solid ground tile 2 is ground that you can stand on but go through
void CheckCollisions(void){
   
	while(anycollision(1, mario.hitbox)==1){ // Collision on the left of the sprite...
		mario.x+= norm_fix(1); // Move by 1 pixel...
	}
	while(anycollision(1, mario.hitbox)==2){ // Collision on the right of the sprite...
		mario.x-= norm_fix(1); // Move by 1 pixel...
	}
	while(anycollision(1,mario.hitbox)==3){ // Collision with the something on top
	   mario.y+=norm_fix(1);
	   mario.vy=0;
	} 
	while(anycollision(1,mario.hitbox)==4 || anycollision(2,mario.hitbox)==4){ // Collision on the bottom of the sprite...
		mario.y -= norm_fix(1); // Move by 1/2 pixel...
	}
	  
	
	if( (TouchingGround()==1 || TouchingGround()==2) && mario.vy>0){ //if he's touching the ground and moving down he can't hit ground while moving up
		mario.vy = 0;
		mario.jumping=0;
	}
		
	//coin collisions
	int i;
	for(i=0;i<maxcoins;i++){
	   if(coin[i].sprite!=0){
		   if(collision(coin[i].x,coin[i].y,8,8)){
		      coin[i].alive=false;
		      deletesprite(coin[i].sprite);
		      coin[i].sprite=0;
			}
		}
	}									
	
}

//places coins whenever it finds yellow
void placecoins(){
   int i;
   int j;
   int coinnumber=0;
   
   for(i=0;i<levellength;i++){
      for(j=0;j<levelheight;j++){
         if(GetTile(i*8,j*8)==3){
            coin[coinnumber].x=i*8;
            coin[coinnumber].y=j*8;
            coin[coinnumber].alive=1;
            PA_OutputText(1,1,coinnumber,"x: %d, y: %d",coin[coinnumber].x, coin[coinnumber].y);
            if(coin[coinnumber].x<=fix_norm(mario.scrollx)+256 && coin[coinnumber].x>=fix_norm(mario.scrollx)){
               coin[coinnumber].sprite=getsprite();
            	PA_CreateSprite(0, coin[coinnumber].sprite,(void*)coin_Sprite, OBJ_SIZE_8X8,1, 0, coin[coinnumber].x, coin[coinnumber].y);
          	}  	
            coinnumber++;
         }
		}	
	}
}


u8 getsprite(){
   int i;
   for(i=0;i<128;i++){
      if(sprite[i]==0){
         sprite[i]=1;
			return i;
		}			
   }
	return -1;
}

void deletesprite(u8 spritenumber){
	sprite[spritenumber]=0;
	PA_DeleteSprite(0,spritenumber);
}
	
