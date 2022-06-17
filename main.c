#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "utils.h"

int main()
{
	curs_set(0);
	int row, col, ch, y = 1, x = 1;
	char ctrls[] = "MOVEMENT: wasd, MAKE WALL: h, MAKE ENEMY: j, MAKE MOVEABLE BALL: k, MAKE TARGET: l, START LEVEL: q";
	int i, j;
	int walcnt = 1;
	int enecnt = 1;
	int tarcnt = 1;
	int tardon = 0;
	int movcnt = 1;
	int* waly = malloc(sizeof(int));
	int* walx = malloc(sizeof(int));
	int* movy = malloc(sizeof(int));
	int* movx = malloc(sizeof(int));
	int* eney = malloc(sizeof(int));
	int* enex = malloc(sizeof(int));
	int* enediff = malloc(sizeof(int));
	int* tary = malloc(sizeof(int));
	int* tarx = malloc(sizeof(int));
	srandom(time(NULL));

	initscr();
	raw();
	palette(); //initalizes colors
	printw("\nHow wide would you like your window to be?\n");
	scanw("%d", &col);
	printw("\nHow long would you like your window to be?\n");
	scanw("%d", &row);
	if(row > LINES || col > COLS)
	{
		endwin();
		fprintf(stderr, "\nVALUES ENTERED WERE TOO LARGE: MAX WIDTH = %d, MAX LENGTH = %d\n", COLS, LINES);
		return -1;
	}
	mvprintw(LINES - 1, (COLS - (strlen(ctrls))) / 2, "%s", ctrls);
	WINDOW * mywin = newwin(row, col,  (LINES - row) / 2, (COLS - col) / 2); //creates window based on user input if the user tries to make the box too large the program will stall indefinitely 
	refresh();
	while(ch != 'q')
	{
		curs_set(0);
		ch = wgetch(mywin);
		wclear(mywin);
		wrefresh(mywin);
		if(ch == 'w' || ch == 'a' || ch == 's' || ch == 'd')
		{
			mvmnt(row, col, &y, &x, ch);
		}
		switch(ch)
		{
			case 'h' :
				walx = realloc(walx, sizeof(int)*walcnt); //this function dynamically allocated memory for wall positions
				walx[walcnt - 1] = x;
				waly = realloc(waly, sizeof(int)*walcnt);
				waly[walcnt - 1] = y;
				walcnt++;
				break;
			case 'j' :
				curs_set(1);
				mvwprintw(stdscr, 7, 0, "How difficult do you want it to be? 1-5: ");
				wscanw(stdscr, "%d", (enediff + (enecnt -1)));
				curs_set(0);
				enex = realloc(enex, sizeof(int)*enecnt); //this function dynamically allocates memory for enemy positions and movement directions
				enex[enecnt - 1] = x;
				eney = realloc(eney, sizeof(int)*enecnt);
				eney[enecnt - 1] = y;
				enecnt++;
				break;
			case 'k' :
				movx = realloc(movx, sizeof(int)*movcnt); //this function dynamically allocated memory for wall positions
				movx[movcnt - 1] = x;
				movy = realloc(movy, sizeof(int)*movcnt);
				movy[movcnt - 1] = y;
				movcnt++;
				break;
			case 'l' :
				tarx = realloc(tarx, sizeof(int)*tarcnt); //this function dynamically allocated memory for wall positions
				tarx[tarcnt - 1] = x;
				tary = realloc(tary, sizeof(int)*tarcnt);
				tary[tarcnt - 1] = y;
				tarcnt++;
				break;
		}	
		wattron(mywin, COLOR_PAIR(6));
		box(mywin, 0, 0);
		wattroff(mywin, COLOR_PAIR(6));
		wattron(mywin, COLOR_PAIR(4));
		mvwprintw(mywin, y, x, "#");
		wattroff(mywin, COLOR_PAIR(4));
		wattron(mywin, COLOR_PAIR(2));
		for(i = 0; i < walcnt - 1; i++)
		{
			mvwprintw(mywin, waly[i], walx[i], "&");
		}
		wattroff(mywin, COLOR_PAIR(2));
		wattron(mywin, COLOR_PAIR(5));
		for(i = 0; i < enecnt - 1; i++)
		{
			mvwprintw(mywin, eney[i], enex[i], "$");
		}
		wattroff(mywin, COLOR_PAIR(5));
		wattron(mywin, COLOR_PAIR(3));
		for(i = 0; i < movcnt - 1; i++)
		{
			mvwprintw(mywin, movy[i], movx[i], "@");
		}
		wattroff(mywin, COLOR_PAIR(3));
		wattron(mywin, COLOR_PAIR(1));
		for(i = 0; i < tarcnt - 1; i++)
		{
			mvwprintw(mywin, tary[i], tarx[i], "X");
		}
		wattroff(mywin, COLOR_PAIR(1));
		wrefresh(mywin);
	}
	wclear(mywin);
	ch = '\0';
	while(ch != 'q')
	{
		halfdelay(2);
		ch = wgetch(mywin);
		wclear(mywin);
		wrefresh(mywin);
		mvmnt(row, col, &y, &x, ch);
		wallcheck(&y, waly, &x, walx, walcnt, ch);
		movcheck(&y, movy, &x, movx, ch, row, col, movcnt);
		for(i = 0; i < movcnt - 1; i++)
		{
			wallcheck(movy + i, waly, movx + i, walx, walcnt, ch);
		}
		compcheck(tary, tarx, &tardon, tarcnt, movy, movx, movcnt);
		chase(y, waly, x, walx, eney, enex, enediff, enecnt, walcnt, (const int)col);
		wattron(mywin, COLOR_PAIR(6));
		box(mywin, 0, 0);
		wattroff(mywin, COLOR_PAIR(6));
		wattron(mywin, COLOR_PAIR(4));
		mvwprintw(mywin, y, x, "#");
		wattroff(mywin, COLOR_PAIR(4));
		wattron(mywin, COLOR_PAIR(2));
		for(i = 0; i < walcnt - 1; i++)
		{
			mvwprintw(mywin, waly[i], walx[i], "&");
		}
		wattroff(mywin, COLOR_PAIR(2));
		wattron(mywin, COLOR_PAIR(5));
		for(i = 0; i < enecnt - 1; i++)
		{
			mvwprintw(mywin, eney[i], enex[i], "$");
		}
		wattroff(mywin, COLOR_PAIR(5));
		wattron(mywin, COLOR_PAIR(3));
		for(i = 0; i < movcnt - 1; i++)
		{
			mvwprintw(mywin, movy[i], movx[i], "@");
		}
		wattroff(mywin, COLOR_PAIR(3));
		wattron(mywin, COLOR_PAIR(1));
		for(i = 0; i < tarcnt - 1; i++)
		{
			mvwprintw(mywin, tary[i], tarx[i], "X");
		}
		wattroff(mywin, COLOR_PAIR(1));
		for(i = 0; i < enecnt - 1; i++)
		{
			if(eney[i] == y && enex[i] == x)
			{
				cbreak();
				wclear(mywin);
				wattron(mywin, COLOR_PAIR(5));
				mvwprintw(mywin, row / 2, col / 2, "YOU LOSE");
				wattroff(mywin, COLOR_PAIR(5));
				wgetch(mywin);
				ch = 'q';
			}
		}
		if(tardon == tarcnt - 1 && tarcnt - 1 != 0)
		{
			cbreak();
			wclear(mywin);
			wattron(mywin, COLOR_PAIR(1));
			mvwprintw(mywin, row / 2, col / 2, "YOU WIN");
			wattroff(mywin, COLOR_PAIR(1));
			wgetch(mywin);
			ch = 'q';
		}
		else
		{
			tardon = 0;
		}
		wrefresh(mywin);
	}
	free(waly);
	free(walx);
	free(eney);
	free(enex);
	free(enediff);
	free(movy);
	free(movx);
	free(tary);
	free(tarx);
	endwin();
	return 0;
}

void mvmnt(int row, int col, int *y, int *x, int ch)
{
	switch(ch) //implements basic character movement based on wasd user input
	{
		case 'w' :
			(*y)--;
			if(*y < 1) //this if statement and similar following ones stop the character from leaving the screen
			{
				(*y)++;
			}
			break;
		case 'a' :
			(*x)--;
			if(*x < 1)
			{
				(*x)++;
			}
			break;
		case 's' :
			(*y)++;
			if(*y > row - 2)
			{
				(*y)--;
			}
			break;
		case 'd' :
			(*x)++;
			if(*x > col - 2)
			{
				(*x)--;
			}
			break;
	}
}

void palette()
{
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);
}

void chase(int y, int* waly, int x, int* walx, int* eney, int* enex, int* enediff, int enecnt, int walcnt, const int col)
{
	int i, j, randtim, randmv;
	for(i = 0; i < enecnt - 1; i++)
	{
		randmv = random() % 4;
		randtim = random() % (7 - (enediff[i])); //random movement timer of enemy decided based on user defined difficulty
		if(randtim == 0 && abs(y-(eney[i])) + abs(x-(enex[i])) < col)
		{
			if(abs(y-(eney[i])) > abs(x-(enex[i])))
			{
				if(y > eney[i])
				{
					(eney[i])++;
					for(j = 0; j < walcnt - 1; j++) //implements wall collisions for enemys
					{
						if(eney[i] == waly[j] && enex[i] == walx[j])
						{
							eney[i]--;
							if(random() % 3 == 0)
							{
								switch(randmv) //adds occasional random movement if enemy runs it a wall to prevent them from getting trapped sometimes allows them to phase through walls
								{
									case 0 :
										enex[i]--;
										break;
									case 1 :
										enex[i]++;
										break;
									case 2 :
										eney[i]--;
										break;
								}
							}
							if(eney[i] == waly[j] && enex[i] == walx[j])
							{
								switch(randmv)
								{
									case 0 :
										enex[i]++;
										break;
									case 1 :
										enex[i]--;
										break;
									case 2 :
										eney[i]++;
										break;
								}
							}
							j = walcnt - 1; //this statement and similar one stop the for loop from continuing to check something if that thing can only happen once and has already happened
						}
					}
				}
				else
				{
					(eney[i])--;
					for(j = 0; j < walcnt - 1; j++)
					{
						if(eney[i] == waly[j] && enex[i] == walx[j])
						{
							eney[i]++;
							if(random() % 3 == 0)
							{
								switch(randmv)
								{
									case 0 :
										enex[i]--;
										break;
									case 1 :
										enex[i]++;
										break;
									case 2 :
										eney[i]++;
										break;
								}
							}
							if(eney[i] == waly[j] && enex[i] == walx[j])
							{
								switch(randmv)
								{
									case 0 :
										enex[i]++;
										break;
									case 1 :
										enex[i]--;
										break;
									case 2 :
										eney[i]--;
										break;
								}
							}
							j = walcnt - 1;
						}
					}
				}
			}
			else
			{
				if(x > enex[i])
				{
					(enex[i])++;
					for(j = 0; j < walcnt - 1; j++) //implements wall collisions for enemys
					{
						if(eney[i] == waly[j] && enex[i] == walx[j])
						{
							enex[i]--;
							if(random() % 3 == 0)
							{
								switch(randmv)
								{
									case 0 :
										enex[i]--;
										break;
									case 1 :
										eney[i]--;
										break;
									case 2 :
										eney[i]++;
										break;
								}
							}
							if(eney[i] == waly[j] && enex[i] == walx[j])
							{
								switch(randmv)
								{
									case 0 :
										enex[i]++;
										break;
									case 1 :
										eney[i]++;
										break;
									case 2 :
										eney[i]--;
										break;
								}
							}
							j = walcnt - 1;
						}
					}
				}
				else
				{
					(enex[i])--;
					for(j = 0; j < walcnt - 1; j++) //implements wall collisions for enemys
					{
						if(eney[i] == waly[j] && enex[i] == walx[j])
						{
							enex[i]++;
							if(random() % 3 == 0)
							{
								switch(randmv)
								{
									case 0 :
										enex[i]++;
										break;
									case 1 :
										eney[i]--;
										break;
									case 2 :
										eney[i]++;
										break;
								}
							}
							if(eney[i] == waly[j] && enex[i] == walx[j])
							{
								switch(randmv)
								{
									case 0 :
										enex[i]--;
										break;
									case 1 :
										eney[i]++;
										break;
									case 2 :
										eney[i]--;
										break;
								}
							}
							j = walcnt - 1;
						}
					}
				}
			}
		}
	}
}

void wallcheck(int *y, int *waly, int *x, int *walx, int walcnt, int ch)
{
	int i;
	switch(ch) //prevents the player from entering a wall
	{
		case 'w' ://this logic and loop and similar following ones send character back one space if they attempt to enter a wall according to the direction they were going before
			for(i = 0; i < walcnt - 1; i++)
			{
				if((*y) == waly[i] && (*x) == walx[i])
				{
					(*y)++;
					i = walcnt - 1;
				}
			}
			break;
		case 'a' :
			for(i = 0; i < walcnt - 1; i++)
			{
				if((*y) == waly[i] && (*x) == walx[i])
				{
					(*x)++;
					i = walcnt - 1;
				}
			}
			break;
		case 's' :
			for(i = 0; i < walcnt - 1; i++)
			{
				if((*y) == waly[i] && (*x) == walx[i])
				{
					(*y)--;
					i = walcnt - 1;
				}
			}
			break;
		case 'd' :
			for(i = 0; i < walcnt - 1; i++)
			{
				if((*y) == waly[i] && (*x) == walx[i])
				{
					(*x)--;
					i = walcnt - 1;
				}
			}
			break;
	}
}

void compcheck(int* tary, int* tarx, int* tardon, int tarcnt, int* movy, int* movx, int movcnt) //checks if the user has won yet by checking if all targets have balls on them
{
	int i, j;
	for(i = 0; i < tarcnt - 1; i++)
	{
		for(j = 0; j < movcnt - 1; j++)
		{
			if(tary[i] == movy[j] && tarx[i] == movx[j])
			{
				(*tardon)++;
			}
		}
	}
}

void movcheck(int* y, int* movy, int* x, int* movx, int ch, int row, int col, int movcnt) //this function allows the player to push moveable balls
{
	int i;
	for(i = 0; i < movcnt - 1; i++)
	{
		if(*x == movx[i] && *y == movy[i])
		{
			switch(ch)
			{	
				case 'a' :
					(movx[i])--;
					break;
				case 's' :
					(movy[i])++;
					break;
				case 'w' :
					(movy[i])--;
					break;
				case 'd' :
					(movx[i])++;
					break;
			}
			if(movy[i] < 2)
			{
				(*y)++;
				(movy[i])++;
			}
			if(movx[i] < 2)
			{
				(*x)++;
				(movx[i])++;
			}
			if(movx[i] > col - 3)
			{
				(*x)--;
				(movx[i])--;
			}
			if(movy[i] > row - 3) 
			{
				(*y)--;
				(movy[i])--;
			}
		i = movcnt - 1;
		}	
	}
}	
