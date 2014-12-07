//sup3rgh0st
//math_tools.c
//12/5/14

#include <3ds.h>
#include <stdlib.h>
#include "math_tools.h"

//Return number between 1-20
int roll_d20(){
	return rand() % 20 + 1;
}

//Return number between 0-1
int coin_flip(){
	return rand() % 1;
}