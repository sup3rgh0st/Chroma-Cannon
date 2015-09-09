//sup3rgh0st
//graphic_tools.c
//12/5/14


#include <3ds.h>
#include "graphic_tools.h"
#include "ascii64.h"

const int TOP_HEIGHT = 240;
const int TOP_WIDTH = 400;
const int BOTTOM_HEIGHT = 240;
const int BOTTOM_WIDTH = 320;

void paint_pixel(u8* fb, u16 x, u16 y, u8 red, u8 green, u8 blue) {
	if ((x>=400) || (x<0)) {return;}
    if ((y>=240) || (y<0)) {return;}
	
	fb[3*(y+x*240)] = blue;
	fb[3*(y+x*240)+1] = green;
	fb[3*(y+x*240)+2] = red;
}

void paint_rect(u8* fb, u16 x1, u16 y1, u16 x2, u16 y2, u8 red, u8 green, u8 blue) {
	int i=0;
	int k=0;
	for(i = x1;i <= x2; i++){
		for(k = y1; k <= y2; k++){
			paint_pixel(fb,i,k,red,green,blue);
		}
	}
}

void paint_rect_c(u8* fb, u16 x1, u16 y1, u16 x2, u16 y2,Color c) {
	int i=0;
	int k=0;
	for(i = x1;i <= x2; i++){
		for(k = y1; k <= y2; k++){
			paint_pixel(fb,i,k,c.red,c.green,c.blue);
		}
	}
}

void draw_line( int x1, int y1, int x2, int y2, char r, char g, char b, u8* fb){
	int x, y;
	if (x1 == x2){
		if (y1<y2)
			for (y = y1; y < y2; y++)
				paint_pixel(fb,x1,y,r,g,b);
		else
			for (y = y2; y < y1; y++)
				paint_pixel(fb,x1,y,r,g,b);
	}
	else{
	if (x1<x2)
		for (x = x1; x < x2; x++)
			paint_pixel(fb,x,y1,r,g,b);
	else
		for (x = x2; x < x1; x++)
			paint_pixel(fb,x,y1,r,g,b);
	}
}

void draw_char(char letter,int x,int y, u8 r, u8 g, u8 b, u8* fb){
  int i, k;
  unsigned char mask;
  unsigned char l;

  for (i = 0; i < 8; i++){
    mask = 0b10000000;
    l = ascii_data[letter][i];
    for (k = 0; k < 8; k++){
      if ((mask >> k) & l){
		paint_pixel(fb,k+x,y-i,r,g,b);
      }     
    }
  }
}

void draw_string(char* s,int x, int y,  u8 r, u8 g, u8 b, u8* fb){
	int i;
	for(i=0; i<strlen(s);i++){
		draw_char(s[i],x + i*8 + 1, y,r,g,b,fb);
	}
}

void draw_string_size(char* s,int size,int x, int y,  u8 r, u8 g, u8 b, u8* fb){
	int i;
	for(i=0; i<strlen(s);i++){
		draw_char_size(s[i],size,x + i*8*size + 1, y,r,g,b,fb);
	}
}

void draw_char_size(char letter,int size,int x,int y, u8 r, u8 g, u8 b, u8* fb){
  int i, k, e, a;
  unsigned char mask;
  unsigned char l;

  for (i = 0; i < 8; i++){
    mask = 0b10000000;
    l = ascii_data[letter][i];
    for (k = 0; k < 8; k++){
      if ((mask >> k) & l){
		for(e = 0; e < size; e++){
			for(a = 0; a < size; a++){
				paint_pixel(fb,k*size+x+e ,y-i*size+a ,r,g,b);
			}
		}
      }     
    }
  }
}



