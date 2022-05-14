#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "pipeheightgen.h"

int flappymoved;

void* flappymv(void * arg){
	while(1){
		getch();
		flappymoved++;
	}
}

int flappyclscheck(int y, int a){	//Collision Check
	int b = 34 - (27-a);
	if(y <= a || y >= b){
		printw("flappypipea");
		return 1;
	}
	return 0;
}

void mainmenu(){
	wbkgd(stdscr,COLOR_PAIR(6));
	WINDOW * mainmenu = newwin(32,36,1,1);
	wbkgd(mainmenu,COLOR_PAIR(7));
	wattron(mainmenu,COLOR_PAIR(7));
	box(mainmenu,0,0);
	mvwprintw(mainmenu,1,1,"FLAPPY");
	mvwprintw(mainmenu,2,1,"A Libre Flappy-type");
	mvwprintw(mainmenu,3,1,"game by ACT");
	mvwprintw(mainmenu,5,1,"PRESS ANY KEY TO BEGIN");
	mvwprintw(mainmenu,8,1,"Version 1.0");
	mvwprintw(mainmenu,9,1,"Copyright 2022 ACT");
	mvwprintw(mainmenu,10,1,"Licensed under the");
	mvwprintw(mainmenu,11,1,"GNU GPL 3.0");
	mvwprintw(mainmenu,12,1,"https://github.com/act17/flappy");
	refresh(); wrefresh(mainmenu);
	getch();
}

int main(){

	//Init NCurses
	initscr();
	noecho();
	cbreak();
	start_color();
	
	//Init time
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 83333333;				//Gamespeed. Every 0.83 seconds a frame is performed;
							//Gamespeed is 15 FPS.
	//Init Colors
	init_pair(1,COLOR_BLUE,COLOR_BLUE);		//Sky color
	init_pair(2,COLOR_WHITE,COLOR_WHITE);		//Ground color
	init_pair(3,COLOR_YELLOW,COLOR_YELLOW);		//Flappy color
	init_pair(4,COLOR_GREEN,COLOR_GREEN);		//Pipe color
	init_pair(5,COLOR_BLACK,COLOR_BLACK);
	init_pair(6,COLOR_CYAN,COLOR_CYAN);		//Color for main menu
	init_pair(7,COLOR_BLACK,COLOR_WHITE);		//And game over screen.

	//Main Menu Function (Added after game was developed.)
	mainmenu();

	//Init windows
	WINDOW * sky = newwin(34,72,1,0);
	WINDOW * ground = newwin(1,72,35,0);
	WINDOW * roof = newwin(1,72,0,0);
	WINDOW * flappy = newwin(1,1,18,6);

	//Init PThread
	pthread_t flappymove;

	//Apply colors to window
	wbkgd(stdscr,COLOR_PAIR(5));
	wbkgd(sky,COLOR_PAIR(1)); wbkgd(ground,COLOR_PAIR(2));
	wbkgd(flappy,COLOR_PAIR(3)); wbkgd(roof,COLOR_PAIR(2));
	wattron(ground,COLOR_PAIR(5));
	refresh(); wrefresh(sky); wrefresh(ground); wrefresh(roof); wrefresh(flappy);

	int flappyhit = 0;		//Numeric track if flappy is hit.
	int pipeheight,pipebheight;	//Value of the height of a new pipe and 'Compliment' to pipeheight's height.
	int score = 0;			//Numeric track of pipes passed.
	int pipemoves = 1;		//Numeric count of pipe movements to see if the pipe should be deleted.
	int flappyheight = 18;		//Numeric track of Flappy's height.

	do{				//This loop will end when Flappy is hit.
			pipeheight = pipeheightgenerator();
			WINDOW * pipea = newwin(pipeheight,1,4,71);
			WINDOW * pipeb = newwin(28-pipeheight,1,1,71);
			wbkgd(pipea,COLOR_PAIR(4));	wbkgd(pipeb,COLOR_PAIR(4));
			wrefresh(pipea);		wrefresh(pipeb);

			pipebheight = 34 - (27-pipeheight);
	while(1){

		if(pipemoves == 72){
			wbkgd(pipea,COLOR_PAIR(1));	wbkgd(pipeb,COLOR_PAIR(1));
			wrefresh(pipea);		wrefresh(pipeb);
			delwin(pipea);			delwin(pipeb);
			pipemoves = 1;
			break;
		}
		
		wbkgd(pipea,COLOR_PAIR(1));	wrefresh(pipea);		//This 'erases' the pipe's prescence onscreen.
		mvwin(pipea,1,71-pipemoves);	wbkgd(pipea,COLOR_PAIR(4));	//This makes the pipe appear in a new position.
		wrefresh(pipea);
		
		wbkgd(pipeb,COLOR_PAIR(1));	wrefresh(pipeb);
		mvwin(pipeb,pipebheight,71-pipemoves);	wbkgd(pipeb,COLOR_PAIR(4));
		wrefresh(pipeb);
		
		pipemoves++;
		flappyheight++;							//This decreases flappy's height by one.

		flappymoved = pthread_create(&flappymove, NULL, flappymv, NULL);
		nanosleep(&ts,&ts);
		pthread_cancel(flappymove);

		if(flappymoved == 1){						//If user made an input during this frame...
			flappyheight = flappyheight - 6;			//Raise Flappy's positon by 5 units.	
		}

		wbkgd(flappy,COLOR_PAIR(1));	wrefresh(flappy);
		mvwin(flappy,flappyheight,6);	wbkgd(flappy,COLOR_PAIR(3));
		wrefresh(flappy);
		
		if(flappyheight == 1 || flappyheight == 36){			//If Flappy hits the roof or ground,
			flappyhit = 1;						//Tell program Flappy was hit,
			break;							//And break the game loop.
		}	
		if(pipemoves == 66){						//If pipes are at Flappy's x-axis
			flappyhit = flappyclscheck(flappyheight,pipeheight);	//Perform a collision check.
			if(flappyhit != 0)					//If Flappy was hit,
				break;						//Break the game loop.
			score++;						//Else, add 1 to the score.
		}

	}
	}while(flappyhit == 0);
	clear();
	WINDOW * gameoverwin = newwin(32,36,1,1);
	wbkgd(stdscr,COLOR_PAIR(6));
	wbkgd(gameoverwin,COLOR_PAIR(7));
	wattron(gameoverwin,COLOR_PAIR(7));
	box(gameoverwin,0,0);
	
	mvwprintw(gameoverwin,2,2,"GAME OVER!");
	mvwprintw(gameoverwin,3,2,"You lost.");
	mvwprintw(gameoverwin,5,2,"Your score:");
	mvwprintw(gameoverwin,6,2,"   %d",score);
	mvwprintw(gameoverwin,9,2,"PRESS ANY");
	mvwprintw(gameoverwin,10,2,"KEY TO EXIT");
	
	refresh(); wrefresh(gameoverwin);
	getch();

	endwin();
	return 0;
}
