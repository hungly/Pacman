#ifndef CELL
#define CELL

struct cell
{
	int value;
	int pred;
	char color;
	int adjs[4];
};

#endif