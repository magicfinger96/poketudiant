#ifndef MAP_H
#define MAP_H

#define SIZEMAXLINE 100
#define MAXCELL 50 /*number of column and lines available for the map */
#define MAXCONSEIL 3 /*number of player in the "conseil des diplomes" */

typedef struct Map Map; 
struct Map{
	int nbCol;
	int nbLines;
	Team *conseil[MAXCONSEIL];
  char *tab[MAXCELL][MAXCELL];
	int pos[2]; /* position of the player on the map ([x,y]) */
};

Team* createTeamConseil(FILE* fichier,Attack tabAtt[NBTYPATT]);
void displayMap(Map *map);
Map* createMap(FILE* fichier,Attack tabAtt[NBTYPATT]);
void freeMap(Map* map);

#endif
