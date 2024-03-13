//-----------------------------------------------------//
// Nintendo e-Reader "Custom Background" Example (GBA) //
// (c) 2004 Tim Schuerewegen                           //
//-----------------------------------------------------//

#include "def.h"
#include "erapi.h"
#include "posprintf.h"

extern int __end[];

#include "background.bmp.raw.c"
#include "background.bmp.pal.c"
#include "background.bmp.map.c"



ERAPI_BACKGROUND background =
{
  background_gfx,
  background_pal,
  background_map,
  sizeof( background_gfx) >> 5,
  sizeof( background_map) >> 5
};


#include "beta.bmp.pal.c"
#include "beta.bmp.raw.c"
ERAPI_SPRITE beta_sprite = { beta_gfx, beta_pal, 8, 8, 1, 1, 8, 8, 1};
u32 beta_pos_x, beta_pos_y, beta_size;
ERAPI_HANDLE_SPRITE handle_beta;
ERAPI_HANDLE_SPRITE handle_sam;

#include "nip.bmp.pal.c"
#include "nip.bmp.raw.c"
ERAPI_SPRITE nip_sprite = { nip_gfx, nip_pal, 1, 1, 1, 1, 8, 8, 1};
ERAPI_HANDLE_SPRITE handle_nips[25];
u32 nip_counter = 0;
u32 nip_pos_x[25], nip_pos_y[25];


char keys[] = "RightLeft Up   Down R    L    ";
char word[6];
#define ERAPI_KEY_A      0x0001
#define ERAPI_KEY_B      0x0002
#define ERAPI_KEY_SELECT 0x0004
#define ERAPI_KEY_START  0x0008
#define ERAPI_KEY_RIGHT  0x0010
#define ERAPI_KEY_LEFT   0x0020
#define ERAPI_KEY_UP     0x0040
#define ERAPI_KEY_DOWN   0x0080
#define ERAPI_KEY_R      0x0100
#define ERAPI_KEY_L      0x0200



const u16 palette[] = { 0x0000, 0xFFFF, 0x0000, 0x0FF0, 0x00F0 };

int main()
{
  ERAPI_HANDLE_REGION region, region2, region3;
  u32 key, quit, count;
  u32 apressed = 0;
  u32 bpressed = 0;
  u32 upressed = 0;
  u32 dpressed = 0;
  u32 rpressed = 0;
  u32 random = 0;
  u32 level = 0;
  u32 timer = 0;
  u32 speed = 1;
  u32 pushme = 0;
  u32 lastnips = 0;
  u32 score = 0;
  char mystr[] = "abcdefghijklmnopqrstuvwxyz1234567890";
  // init
  ERAPI_FadeIn( 1);
  ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
  
  ERAPI_SetBackgroundMode( 0);
  ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x04);
  // background

  // loop
  region = ERAPI_CreateRegion( 0, 0, 0xAE, 0x00, 0x0D, 0x1C);
  ERAPI_SetRegionColor(region, 0);
  region2 = ERAPI_CreateRegion( 1, 0, 0x01, 0x00, 0x0D, 0x1C);
	ERAPI_SetRegionColor(region2, 0);
  
  
  
  //region2 = ERAPI_CreateRegion( 0, 0, 0x00, 0x00, 0x01, 0x01);
  //region3 = ERAPI_CreateRegion( 0, 0, 0x00, 0x00, 0x01, 0x01);
  //17
  
  handle_beta = ERAPI_SpriteCreateCustom( 0, &beta_sprite);
  beta_pos_x = 120;
  beta_pos_y = 120;
  ERAPI_SetSpritePos( handle_beta, beta_pos_x, beta_pos_y);
  
  //ERAPI_PlaySoundSystem(190);
ERAPI_PlaySoundSystem(160);
	// init cursor string

  count = 0;
  quit = 0;
  //mystr[0] = 'a';
  //mystr[1] = 'b';
	//handle_sam = ERAPI_SpriteCreateSystem(1, 58);
	//ERAPI_SetSpritePos( handle_sam, 120, 120);


	for(u32 i=0; i<25; i++){
		handle_nips[i] =ERAPI_SpriteCreateCustom( 1, &nip_sprite);
		nip_pos_x[i] = 60;
		nip_pos_y[i]= 165;
		ERAPI_SetSpritePos( handle_nips[i], nip_pos_x[i], nip_pos_y[i]);
	}


  while (quit == 0)
  {
	key = ERAPI_GetKeyStateRaw();
	if (key & ERAPI_KEY_START){
		level = 1;
		count = 0;
		timer = 0;
		ERAPI_ClearRegion(region);
		ERAPI_ClearRegion(region2);
		ERAPI_LoadBackgroundCustom( 3, &background);
		
		ERAPI_SetBackgroundAutoScroll( 3, 0x00, 0x00);
		ERAPI_SetBackgroundOffset(3, 0x00,0x00);
		ERAPI_SoundPause(160); 
		ERAPI_PlaySoundSystem(190);
		ERAPI_SetTextColor( region, 0x02, 0x00);
		ERAPI_DrawText( region, 0x00, 0x0A, "Cheesy Nip Clicker");
		//ERAPI_DrawText( region2, 0x00, 0x0A, "Cheesy Nip Clicker");
		beta_pos_x = 50;
		beta_pos_y = 60;
		
		for(u32 i=0; i<25; i++){
			nip_pos_x[i] = 50;
			nip_pos_y[i] =164;
		}
		
	}
	if (key & ERAPI_KEY_SELECT) quit = 1;
	  
	if (level == 9){
		//Game over!
	  count = 0;
	  ERAPI_SetTextColor( region, 0x01, 0x00);
	  ERAPI_DrawText( region, 0x00, 0x08, "Game Over");
	  if (timer < 99999){
		score = 99999 - timer;
	  } else {
     	score = 0;
	  }
	  posprintf(mystr, "Score: %l", score);	
	  ERAPI_DrawText( region, 0x00, 0x10, mystr);
	  ERAPI_DrawText( region, 0x00, 0x18, "Press Start To");
	  ERAPI_DrawText( region, 0x00, 0x20, "Play Again");
	  // background
	  ERAPI_LoadBackgroundSystem( 3, 2);
	  ERAPI_SetBackgroundAutoScroll( 3, 0x40, 0x40);
	  beta_pos_x = 120;
	  beta_pos_y = 120;
	  
	} else if (level == 0) {
	  ERAPI_SetTextColor( region, 0x01, 0x00);
	  //posprintf(mystr, "%x %x %x", region, region2, region3);	
	  ERAPI_DrawText( region, 0x00, 0x08, "Cheesy Nip Clicker" );
	  ERAPI_DrawText( region, 0x00, 0x18, "Press Start");
	  ERAPI_DrawText( region, 0x00, 0x20, "To Play!");
	  // background
	  ERAPI_LoadBackgroundSystem( 3, 2);
	  ERAPI_SetBackgroundAutoScroll( 3, 0x40, 0x40);
	  beta_pos_x = 120;
	  beta_pos_y = 120;
	  
	} else {
		
	    //Gamestate START!


		
		timer++;
		speed=1;
		ERAPI_DrawText(region, 0x00, 0x4A, "L1: Click A");
		if (count < 999999999){
			posprintf(mystr, "Level:%d Nips:%d", level, count);	
		}
		ERAPI_DrawText(region, 0x00, 0x2E, mystr);
		
		
		
		//Level 1 - Push A
		if (key & ERAPI_KEY_A && count < 980) {
			if (apressed == 0){
				count++;
				apressed = 1;	
			} 
		} else {
			apressed = 0;
		}
		
		if (key & ERAPI_KEY_UP) {
			upressed = 1;	
		}
		if (key & ERAPI_KEY_DOWN) {
			dpressed = 1;	
		}		
		//Level 2 - Push A and B
		if (count > 19 && level < 2 && count < 950){
			level = 2;
			//White on Black... Or White on Transparent
			ERAPI_DrawText(region, 0x00, 0x52, "L2: Use A+B");
		}
		
		if ((key & ERAPI_KEY_B) && count > 20 && count < 950) {
			if (bpressed == 0){
				count++;
				bpressed = 1;
			} 
		} else {
			bpressed = 0;
		}
		
		
		
		if (count > 99 && level < 3){
			level = 3;
			//White on Green
			ERAPI_DrawText(region, 0x00, 0x5A, "L3: Autonip!");
		}
		
		if (count > 149 && level < 4){
			//black on white or trtansparent.
			ERAPI_DrawText(region, 0x00, 0x62, "L4: L=Faster Auto");
			level = 4;
		}
		
		if ((key & ERAPI_KEY_L) && count > 149) {
			speed = 5;
		}
		
		if (count > 99 && count < 950){
			if (timer % 50 == 1){
				count += speed;
			}
		}
		
		if (count > 249 && level < 5){
			ERAPI_DrawText(region, 0x00, 0x6A, "L5: Up Then Down");
			level = 5;
		}
		
		if (count > 249 && count < 950){
			if (upressed == 1 && dpressed == 1){
				upressed = 0; 
				dpressed = 0;
				count = count + 25;
			}
		}
		
		if (count > 499 && level < 6){
			ERAPI_DrawText(region, 0x00, 0x72, "L6: R Joins!");
			level = 6;
		}
		
		if ((key & ERAPI_KEY_R) && count > 499 && count < 950) {
			if (rpressed == 0){
				count = count + 10;
				rpressed = 1;
			} 
		} else {
			rpressed = 0;
		}
		
		if (count > 949 && level < 7){
			ERAPI_DrawText(region, 0x00, 0x7A, "L7: (A only)");
			level = 7;
		}

		
		if (count > 979 && level < 8){
			random = ERAPI_Rand();
			pushme = ERAPI_Mod(random, 6);
			for (int i=pushme*5; i<(1+pushme)*5; i++){
				word[i%5] = keys[i];
			}
			posprintf(mystr, "Press %s", word);
			ERAPI_DrawText(region2, 0x01, 0x0A, mystr);
			
			//posprintf(mystr, "L8: MIMIC!");	
			//ERAPI_DrawText(region, 0x00, 0x0A, mystr);
			ERAPI_DrawText(region, 0x00, 0x82, "L8: Mimic!");
			level = 8;
		}


		if (count > 979){
			if (key & (1 << (pushme+4))){
				random = ERAPI_Rand();
				pushme = ERAPI_Mod(random, 6);
				for (int i=pushme*5; i<(1+pushme)*5; i++){
					word[i%5] = keys[i];
				}
				posprintf(mystr, "Press %s", word);
				ERAPI_DrawText(region2, 0x01, 0x0A, mystr);
				
				count++;
			}
		}
		
		
	}
	if (lastnips != count){
		lastnips = count;
		nip_pos_y[nip_counter] = 150;
		nip_counter++;
		if (nip_counter > 24) nip_counter = 0;
	}
	
	


	//GameOver Do Absolutely Last in Game Loop!
	if (count > 1000){
		level = 9;
		ERAPI_ClearRegion(region);
		ERAPI_ClearRegion(region2);
		for(u32 i=0; i<25; i++){
			nip_pos_y[i] = 165;
		}
		ERAPI_SetTextColor( region, 0x01, 0x00);
		ERAPI_SoundPause(190);
		ERAPI_PlaySoundSystem(76);
		//ERAPI_SoundPause(160); 
	}
	
	ERAPI_SetSpritePos( handle_beta, beta_pos_x, beta_pos_y);
	for(u32 i=0; i<25; i++){
		if (nip_pos_y[i] < 151){
			//if (timer % 1 == 1){
			nip_pos_y[i]--;
			//}
		}
		if (nip_pos_y[i] < 86){
			nip_pos_y[i] = 165;
		}
		
		ERAPI_SetSpritePos( handle_nips[i], nip_pos_x[i], nip_pos_y[i]);
	}

    ERAPI_RenderFrame(1);
	
  }
  // exit
  return ERAPI_EXIT_TO_MENU;
}
