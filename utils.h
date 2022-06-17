#ifndef utils

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

#define utils
#endif
