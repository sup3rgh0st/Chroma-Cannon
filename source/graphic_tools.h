//sup3rgh0st
//graphic_tools.h
//12/5/14

#ifndef GRAPHIC_TOOLS_H
#define GRAPHIC_TOOLS_H

typedef struct{
	u8 red;
	u8 green;
	u8 blue;
} Color;

void paint_pixel(u8* fb, u16 x, u16 y, u8 red, u8 green, u8 blue);
void paint_rect(u8* fb, u16 x1, u16 y1, u16 x2, u16 y2, u8 red, u8 green, u8 blue);
void paint_rect_c(u8* fb, u16 x1, u16 y1, u16 x2, u16 y2,Color c);
void draw_line( int x1, int y1, int x2, int y2, char r, char g, char b, u8* fb);  //CalebDW - https://github.com/CalebDW/Breakout3DS
void draw_char(char letter,int x,int y, u8 r, u8 g, u8 b, u8* fb); //CalebDW - https://github.com/CalebDW/Breakout3DS
void draw_string(char* s,int x, int y,  u8 r, u8 g, u8 b, u8* fb);
void draw_string_size(char* s,int size,int x, int y,  u8 r, u8 g, u8 b, u8* fb);
void draw_char_size(char letter,int size,int x,int y, u8 r, u8 g, u8 b, u8* fb);
#endif