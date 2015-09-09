/*
	Chroma Cannon
	sup3rgh0st
	5/15/15
*/

#include <3ds.h>
#include <stdio.h>
#include <math.h>
#include "graphic_tools.h"
#include "tool_tools.h"

const int AIM_CENTER_X = 160;
const int AIM_CENTER_Y = 120;
const int BASIC_RAD = 3;
const int BULLET_RAD = 1;
const int PLAYER_PATH_MAX = 51; //MUST BE > 50

int spawnCoolDown;
int time;
int level;
int levelUpTextX;

int pathIndex;

u8 bg_red = 252;
u8 bg_green = 0;
u8 bg_blue = 0;
int bg_state = 1;
int bg_bump = 7;

Color Red   = {255,0,0};
Color Green = {0,255,0};
Color Blue  = {0,0,255};
Color Black = {0,0,0};
Color White = {255,255,255};
Color Steel = {119,139,162};

struct Player{
	int score;
	int coolDown;
	int health;
	int posX;
	int posY;
};
struct Player p;

struct Bullet{
	int life;
	double posX;
	double posY;
	double velX;
	double velY;
};
struct Bullet Ammo[30];

struct BasicEnemy{
	int life;
	double posX;
	double posY;
};
struct BasicEnemy Hoard[50];

struct PlayerPath{
	int posX;
	int posY;
	//int life;
	Color color;
};
struct PlayerPath Path[51];

void resetGame(){
	p.posX = 200;
	p.posY = 120;
	p.coolDown = 0;
	p.health = 100;
	p.score = 0;
	int i;
	for(i = 0; i < 30; i++){
		Ammo[i].life = 0;
		Hoard[i].life = 0;
		//Path[i].life = 0;
		Path[i].posX = -10;
	}
	for(i = 30; i < 50; i++){
		Hoard[i].life = 0;
		//Path[i].life = 0;
		Path[i].posX = -10;
	}
	for(i=50; i < PLAYER_PATH_MAX; i++){
		//Path[i].life = 0;
		Path[i].posX = -10;
	}
	
	pathIndex=0;
	spawnCoolDown = 50;
	time = 0;
	level=0;
	levelUpTextX = -1000;
	bg_state = 1;
	bg_red = 252;
	bg_green = 0;
	bg_blue = 0;
}

int getFreeBulletIndex(){
	int i;
	for(i = 0; i < 30; i++){
		if(Ammo[i].life == 0){
			return i;
		}
	}
	return -1;
}

int getFreeBasicEnemyIndex(){
	int i;
	for(i = 0; i < 50; i++){
		if(Hoard[i].life == 0){
			return i;
		}
	}
	return -1;
}

int main(int argc, char **argv)
{
	//Matrix containing the name of each key. Useful for printing when a key is pressed
	char keysNames[32][32] = {
		"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
		"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
		"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
		"", "", "KEY_ZL", "KEY_ZR",
		"", "", "", "",
		"KEY_TOUCH", "", "", "",
		"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
		"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
	};
	
	//Run Pre-Game Scripts
	resetGame();

	// Initialize services
	gfxInitDefault();

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_BOTTOM, NULL);

	u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0; //In these variables there will be information about keys detected in the previous frame

	printf("\x1b[0;0HChromaCannon v0.2 Press Start to exit.");
	printf("\x1b[1;0HCirclePad position:");
	printf("\x1b[3;0HTouch Screen position:");

	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();
		//hidKeysHeld returns information about which buttons have are held down in this frame
		u32 kHeld = hidKeysHeld();
		//hidKeysUp returns information about which buttons have been just released
		u32 kUp = hidKeysUp();

		if (kDown & KEY_START) break; // break in order to return to hbmenu
		
		u8* fbTop = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		u8* fbBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		memset(fbTop, 0, 240*400*3);

		//Set keys old values for the next frame
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;
		
		
		//Rainbow Handler======
		switch (bg_state){
			case 1:
				bg_blue+= bg_bump;
				if(bg_blue == 252){
					//bg_blue = 254;
					bg_state++;
				}
				break;
			case 2:
				bg_red-= bg_bump;
				if(bg_red ==  0){
					//bg_red = 0;
					bg_state++;
				}
				break;
			case 3:
				bg_green+= bg_bump;
				if(bg_green == 252){
					//bg_green = 254;
					bg_state++;
				}
				break;
			case 4:
				bg_blue-= bg_bump;
				if(bg_blue ==  0){
					//bg_blue = 0;
					bg_state++;
				}
				break;
			case 5:
				bg_red+= bg_bump;
				if(bg_red == 252){
					//bg_red = 254;
					bg_state++;
				}
				break;
			case 6:
				bg_green-= bg_bump;
				if(bg_green ==  0){
					//bg_red = 0;
					bg_state=1;
				}
				break;
		}
		//printf("\x1b[5;0H RGB %u; %u; %u state %u", bg_red,bg_green,bg_blue,bg_state);
		
		Color BG = {bg_red,bg_green,bg_blue};
		Color invBG = {255-bg_red,255-bg_green,255-bg_blue};
		Color fadeBG = {bg_red+100,bg_green+100,bg_blue+100}; 
		/*
		int bg_red_f   = bg_red  -(200-p.health*2);
		int bg_green_f = bg_green-(200-p.health*2);
		int bg_blue_f  = bg_blue -(200-p.health*2);
		
		if(bg_red_f   > 254) bg_red_f   = 254;
		if(bg_green_f > 254) bg_green_f = 254;
		if(bg_blue_f  > 254) bg_blue_f  = 254;
		*/
		printf("\x1b[5;0H HEALTH %u", p.health);
		
		paint_rect(fbTop, 0, 0, 400-1, 240-1, bg_red,bg_green,bg_blue); // Draw Background
		//End Rainbow Handler==
		
		//Score Display
		char bgScore[10];
		itoa(p.score, bgScore);
		int scoreR = bg_red+100;
		if(scoreR > 254) scoreR = 254;
		int scoreG = bg_green+100;
		if(scoreG > 254) scoreG = 254;
		int scoreB = bg_blue+100;
		if(scoreB > 254) scoreB = 254;
		draw_string_size(bgScore,8,25,210,scoreR,scoreG,scoreB, fbTop); // Draw Score
		//
		
		//Level Up Handler
		if((p.score > 2   && level < 1) ||
		   (p.score > 7   && level < 2) ||
		   (p.score > 19  && level < 3) ||
		   (p.score > 49  && level < 4) ||
		   (p.score > 100 && level < 5) ||
		   (level >= 5 && p.score%100 == 0)){
			levelUpTextX = 400;
			level++;
		}
		if(levelUpTextX > -1000){
			levelUpTextX-= 10;
			draw_string_size("LEVEL UP!!",10,levelUpTextX,100,scoreR,scoreG,scoreB, fbTop); //Level Up Text Event
		}
		//
		
		//Path Handler
		
		if(pathIndex >= PLAYER_PATH_MAX - ((100-p.health)/2)){
			pathIndex = 0;
		}
		Path[pathIndex].color = BG;
		//Path[pathIndex].life = PLAYER_PATH_MAX;
		Path[pathIndex].posX = p.posX;
		Path[pathIndex].posY = p.posY;
		int ppi;
		//for(ppi = 0; ppi < PLAYER_PATH_MAX; ppi++){
		//	path[ppi].life--;
		//}
		for(ppi = 0; ppi < PLAYER_PATH_MAX - ((100-p.health)/2); ppi++){
			paint_rect_c(fbTop,Path[ppi].posX-5,Path[ppi].posY-5,Path[ppi].posX+5,Path[ppi].posY+5,Path[ppi].color);
		}
		pathIndex++;
		//

		circlePosition pos;

		//Read the CirclePad position
		hidCircleRead(&pos);
		
		//Print the CirclePad position
		printf("\x1b[2;0H%04d; %04d", pos.dx, pos.dy);
		
		//Move Entities
		int velX = 0;
		int velY = 0;
		
		if(pos.dx >  20 || pos.dy >  20 ||
		   pos.dx < -20 || pos.dy < -20){
			velX = pos.dx / 15;
			velY = pos.dy / 15;
		}
		
		p.posX = p.posX + velX;
		p.posY = p.posY + velY;
		
		if(p.posX < -7  )   p.posX = -7;
		if(p.posX > 407)   p.posX = 407;
		if(p.posY < -7  )   p.posY = -7;
		if(p.posY > 247)   p.posY = 247;
		
		int bEC = 0; // basicEnemyCoutner
		for(bEC = 0; bEC < 50; bEC++){
			if(Hoard[bEC].life != 0){
				int dx = p.posX - Hoard[bEC].posX;
				int dy = p.posY - Hoard[bEC].posY;
				double len = sqrt(pow(dx,2.0) + pow(dy,2.0));
				double trajX = dx / len;
				double trajY = dy / len;
				
				Hoard[bEC].posX += trajX*2;
				Hoard[bEC].posY += trajY*2;
				
				paint_rect_c(fbTop,Hoard[bEC].posX-BASIC_RAD,Hoard[bEC].posY-BASIC_RAD,Hoard[bEC].posX+BASIC_RAD,Hoard[bEC].posY+BASIC_RAD,Steel);
				
				
			}
		}
		
		//
		
		//Draw Field
		paint_rect_c(fbTop,p.posX-5,p.posY-5,p.posX+5,p.posY+5,invBG);//player
		int bullet;
		for(bullet = 0; bullet < 30; bullet++){
			if(Ammo[bullet].life != 0){
				Ammo[bullet].life = Ammo[bullet].life - 1;
				Ammo[bullet].posX = Ammo[bullet].posX + Ammo[bullet].velX *4;
				Ammo[bullet].posY = Ammo[bullet].posY + Ammo[bullet].velY *4;
				paint_rect_c(fbTop,Ammo[bullet].posX-BULLET_RAD,Ammo[bullet].posY-BULLET_RAD,Ammo[bullet].posX+BULLET_RAD,Ammo[bullet].posY+BULLET_RAD,Black);
				
				//Bullet-Basic Enemy Collision Detection
				
				for(bEC = 0; bEC < 50; bEC++){
					if(Ammo[bullet].posX+BULLET_RAD < Hoard[bEC].posX-BASIC_RAD ||
					   Ammo[bullet].posY+BULLET_RAD < Hoard[bEC].posY-BASIC_RAD ||
					   Ammo[bullet].posX-BULLET_RAD > Hoard[bEC].posX+BASIC_RAD ||
					   Ammo[bullet].posY-BULLET_RAD > Hoard[bEC].posY+BASIC_RAD){
						//Miss, Do Nothing
					}
					else{
						if(Hoard[bEC].life > 0){
							Ammo[bullet].life=0;
							Hoard[bEC].life=0;
							p.score++;
							printf("\x1b[6;0H%u", bEC);
						}
					}
				}
			}
		}
		//
		
		if(p.coolDown > 0) p.coolDown--;
		if(spawnCoolDown > 0) spawnCoolDown--;
		time++;
		
		touchPosition touch;

		//Read the touch screen coordinates
		hidTouchRead(&touch);
		
		if( touch.px != 0 && touch.py !=0 ){
			int dtx = touch.px - AIM_CENTER_X;
			int dty = touch.py - AIM_CENTER_Y;
			double len = sqrt(pow(dtx, 2.0) + pow(dty,2.0));
			double trajX = dtx / len;
			double trajY = dty / len;
			int freeBulletIndex = getFreeBulletIndex();
			if(freeBulletIndex != -1 && p.coolDown == 0){
				p.coolDown = 4;
				Ammo[freeBulletIndex].life = 100;
				Ammo[freeBulletIndex].posX = p.posX;
				Ammo[freeBulletIndex].posY = p.posY;
				Ammo[freeBulletIndex].velX = trajX;
				Ammo[freeBulletIndex].velY = -trajY;
			}
		}
		
		//Spawn Baddies
		if(spawnCoolDown == 0){
			int freeBasicIndex = getFreeBasicEnemyIndex();
			if( freeBasicIndex != -1){
				spawnCoolDown = 100 - (level*15);
				if(spawnCoolDown < 1) spawnCoolDown = 1;
				int side = rand()%(4)+1;
				
				switch(side){
				case 1://Top
					Hoard[freeBasicIndex].posX = rand()%400;
					Hoard[freeBasicIndex].posY = -10;
				break;
				case 2://Bottom
					Hoard[freeBasicIndex].posX = rand()%400;
					Hoard[freeBasicIndex].posY = 250;
				break;
				case 3://Left
					Hoard[freeBasicIndex].posX = -10;
					Hoard[freeBasicIndex].posY = rand()%240;
				break;
				case 4://Right
					Hoard[freeBasicIndex].posX = 410;
					Hoard[freeBasicIndex].posY = rand()%240;
				break;
				}
				Hoard[freeBasicIndex].life = 1;
				
				//printf("\x1b[6;0H%03d; %03d", touch.px, touch.py);
			}
			
		}
		//--
		
		//player collision detection
		for(bEC = 0; bEC < 50; bEC++){
			if(Hoard[bEC].posX+BASIC_RAD < p.posX-5 ||
			   Hoard[bEC].posY+BASIC_RAD < p.posY-5 ||
			   Hoard[bEC].posX-BASIC_RAD > p.posX+5 ||
			   Hoard[bEC].posY-BASIC_RAD > p.posY+5){
				//Miss, Do Nothing
			}
			else{
				if(p.health > 0 && Hoard[bEC].life >0)p.health--;
					Hoard[bEC].life = 0;
				if(p.health == 0){ // Death Handler
					p.coolDown = 100;
					char s[10];
					itoa(p.score, s);
					while(p.coolDown > 0){
						p.coolDown--;
						draw_string_size("Score",3,25,160,0,0,0, fbTop);
						draw_string_size(s,3,25,100,0,0,0, fbTop);
						
						// Flush and swap framebuffers
						gfxFlushBuffers();
						gfxSwapBuffers();

						//Wait for VBlank
						gspWaitForVBlank();	
					}
					resetGame();
				}
			}
		}
		//
		
		
		//Print the touch screen coordinates
		printf("\x1b[4;0H%03d; %03d", touch.px, touch.py);

		
		//Cleanup

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	// Exit services
	gfxExit();
	return 0;
}
