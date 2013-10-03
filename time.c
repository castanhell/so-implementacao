#include "time.h"

int time = 0;

void tick(){
	time++;
}

int systime(){
	return time;
}

