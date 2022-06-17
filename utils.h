#ifndef utils

void movcheck(int* y, int* movy, int* x, int* movx, int ch, int row, int col, int movcnt);
void mvmnt(int row, int col, int *y, int *x, int ch);
void compcheck(int* tary, int* tarx, int* tardon, int tarcnt, int* movy, int* movx, int movcnt);
void wallcheck(int *y, int *waly, int *x, int *walx, int walcnt, int ch);
void chase(int y, int* waly, int x, int* walx, int* eney, int* enex, int* enediff, int enecnt, int walcnt, const int col);
void palette();

#define utils
#endif
