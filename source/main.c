//sup3rgh0st
//main.c
//12/5/14

#include <3ds.h>
//#include <stdio.h>
#include "graphic_tools.h"
#include "tool_tools.h"
#include "math_tools.h"

int main()
{
	// Initialize services
	srvInit();
	aptInit();
	hidInit(NULL);
	gfxInit();
	//gfxSet3D(true); // uncomment if using stereoscopic 3D
	
	//Generate Colors=========
	Color Red   = {255,0,0};
	Color Green = {0,255,0};
	Color Blue  = {0,0,255};
	//End Color Generation====
	
	//Allocate Variables======
	u8 bg_red = 200;
	u8 bg_green = 0;
	u8 bg_blue = 0;
	int bg_state = 1;
	int bg_bump = 30;
	
	int box_x = 0;
	
	char* title;
	u64 test = 123456;
	int title_shift_x = 0;
	int title_shift_y = 0;
	//End Variable Allocation=
	
	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		// Your code goes here
		
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		//Exit Program
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
			
		
		// Example rendering code that displays a white pixel
		// Please note that the 3DS screens are sideways (thus 240x400 and 240x320)
		u8* fbTop = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		u8* fbBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		memset(fbTop, 0, 240*400*3);
		
		//Paint Screen===========================
		//Rainbow Handler======
		switch (bg_state){
			case 1:
				bg_blue+= bg_bump;
				if(bg_blue >= 200){
					bg_blue = 254;
					bg_state=2;
				}
				break;
			case 2:
				bg_red-= bg_bump;
				if(bg_red <= 50){
					bg_red = 0;
					bg_state=3;
				}
				break;
			case 3:
				bg_green+= bg_bump;
				if(bg_green >= 200){
					bg_green = 254;
					bg_state++;
				}
				break;
			case 4:
				bg_blue-= bg_bump;
				if(bg_blue <= 50){
					bg_blue = 0;
					bg_state++;
				}
				break;
			case 5:
				bg_red+= bg_bump;
				if(bg_red >= 200){
					bg_red = 254;
					bg_state++;
				}
				break;
			case 6:
				bg_green-= bg_bump;
				if(bg_green <= 50){
					bg_red = 0;
					bg_state=1;
				}
				break;
		}
		//End Rainbow Handler==
		box_x = bg_green;
		paint_rect(fbTop, 0, 0, 400-1, 240-1, bg_red,bg_green,bg_blue);
		paint_rect_c(fbBottom,box_x, 100,box_x+10, 110, Green);
		
		
		//draw_char('R',100,240,250,250, fbTop);
		draw_string_size("Ryan",3,25, 160, 250, 250 ,250, fbTop);
		draw_string_size("was here!",3,25, 100, 250, 250 ,250, fbTop);
		
		//End Patin Screen=======================
		  

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	// Exit services
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}
