#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int pipeheightgenerator(){
	srand(time(0));
	int pipeheight = rand() % 16 + 8;
	return pipeheight;
}
