#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poketudiant.h"
#include "enumtochar.h"
#include "team.h"
#include "pokecafet.h"
#include "fightMain.h"
#include "map.h"
#include "request.h"
#define RED 124
#define BLUE 74
#define BLACK 237
#define PINK 204
#define WHITE 15
#define COLOR(x) (printf("\x1B[48;5;%dm  ",(x)))
#define REINITCOLOR()	(printf("\033[%dm",0))
#define LVLCOLOR {219,218,217,216,215,214,178,142,106,70}

Team* createTeamConseil(FILE* fichier,Attack tabAtt[NBTYPATT]){
	/* create a team from the conseil */
	char line[SIZEMAXLINE];
	char varietChar[20];
	Variety variety;
  Team *team = (Team*)malloc(sizeof(Team));
	int lvl;
	team->pos=0;

	/* add name of the member of the "conseil" */
	fgets(line,SIZEMAXLINE,fichier);
	sscanf(line,"name_player=%s",team->name);

	/* add the teacher of the member in his team */
	fgets(line,SIZEMAXLINE,fichier);
	sscanf(line,"lvl_teacher=%d",&lvl);
	team->pe[team->pos++] = randomPE(lvl,TEACHER,tabAtt);

	/* add the poketudiant of the member to his team */
	while(team->pos < MAXTEAM){

		/* jump the line of "c1_PE1" "c2_PE2" etc */
		fgets(line,SIZEMAXLINE,fichier);

		/* take the variety out */
		fgets(line,SIZEMAXLINE,fichier);
		sscanf(line,"variety=%s",varietChar);
		variety=charToVariety(varietChar);

		/* define lvl */
		fgets(line,SIZEMAXLINE,fichier);
		sscanf(line,"lvl=%d",&lvl);

		/* create poketudiant in the team*/
	  team->pe[team->pos++] = specificPE(lvl,variety,tabAtt);
	}

	return team;
}

void displayMap(Map *map){
	/* display the map */
	int i,j,k,lvlColor[LVLMAX]=LVLCOLOR,val;

	if (map->nbLines == 0 || map->nbCol == 0){
		printf("The size of the map is null !\n");
	}
	printf("\n");
	if (map->nbLines < 9 || map->nbCol > 20){
		/* display the key at the top of the map 
		if there is not enough space at the right */
		for(j=0;j<LVLMAX;j++){
			COLOR(lvlColor[j]);
		}
		REINITCOLOR();
		printf("  wild area (level 1 to %d)\n\n",LVLMAX);
		COLOR(BLACK);
		COLOR(BLUE);
		REINITCOLOR();
		printf(" path\n\n");
		COLOR(PINK);
		REINITCOLOR();
		printf(" nurse\n\n");
		COLOR(RED);
		REINITCOLOR();
		printf(" the \"conseil des diplômes\"\n");
		printf("\n");
	}

	
	/* display the map*/

	for(i=0;i<map->nbLines;i++){
		for(j=0;j<map->nbCol;j++){
			val=atoi(map->tab[i][j]);
			/* if the cell is where i am :*/
			if(map->pos[0]==i && map->pos[1]== j){
				COLOR(WHITE);
			} else {
				if (val<=LVLMAX && val >= 1){
					/* if the cell is a wild zone :*/
					COLOR(lvlColor[val-1]);
				} else {
					if(map->tab[i][j][0] == 'c'){
						/* if the cell is a member of the "conseil des diplomes" :*/
						COLOR(RED);
					} else {
						if(!strcmp(map->tab[i][j],"n")){
							/* if the cell is the nurse :*/
							COLOR(PINK);
						} else {
							/* if the cell is just a path :*/
							if(!strcmp(map->tab[i][j],"bg")){
								COLOR(BLUE);
							} else {
								if(!strcmp(map->tab[i][j],"b")){
									COLOR(BLACK);
								}
							}
						}
					}
				}
			}
			REINITCOLOR();
		}
	
		/* display the key at the right of the map if there's enough space */
		if (map->nbLines >= 9 || map->nbCol <= 20 ){
			switch(i){
				case 1:
					printf("  ");
					for(k=0;k<LVLMAX;k++){
						COLOR(lvlColor[k]);
					}
					REINITCOLOR();
					printf("  wild area (level 1 to %d)\n",LVLMAX);
					break;
				case 3:
					printf("  ");
					COLOR(BLACK);
					COLOR(BLUE);
					REINITCOLOR();
					printf(" path\n");
					break;
				case 5:
					printf("  ");
					COLOR(PINK);
					REINITCOLOR();
					printf(" nurse\n");
					break;
				case 7:
					printf("  ");
					COLOR(RED);
					REINITCOLOR();
					printf(" the \"conseil des diplômes\"\n");
					break;
				case 9:
					printf("  ");
					COLOR(WHITE);
					REINITCOLOR();
					printf(" me\n");
					break;
				default:
					printf("\n");
					break;
			}
		}
	}
	printf("\n");
}

Map* createMap(FILE* fichier,Attack tabAtt[NBTYPATT]){
	/* create the map from the file */
	Map *map=(Map*)malloc(sizeof(Map));
	int i=0,pos=0,li,col;
	char line[SIZEMAXLINE];
	char *tok,*elem;

	/* fill the conseil des diplomés */
	while(i < MAXCONSEIL){
		fgets(line,SIZEMAXLINE,fichier);
		sscanf(line,"c%d_infos:",&pos);
		map->conseil[pos-1]=createTeamConseil(fichier,tabAtt);
		fgets(line,SIZEMAXLINE,fichier);
		i++;
	}

	/* put the map in the table */
	fgets(line,SIZEMAXLINE,fichier);
	sscanf(line,"nb_lignes=%d",&map->nbLines);
	fgets(line,SIZEMAXLINE,fichier);
	sscanf(line,"nb_colonnes=%d",&map->nbCol);
	
	fgets(line,SIZEMAXLINE,fichier);

	for(li=0;li<map->nbLines;li++){
		col=0;
		fgets(line,SIZEMAXLINE,fichier);
		tok = strtok(line," \n");
    while(tok != NULL){
			elem = strdup(tok);
			if(!strcmp("n",elem)){
				map->pos[0]=li;
				map->pos[1]=col;
			}
    	map->tab[li][col++]=elem;
      tok = strtok(NULL," \n");
		}
	}

	return map;
}

void freeMap(Map* map){
	/* free the space allocated for map */
	int i,j;
	for (i=0;i<MAXCONSEIL;i++){
		free_team(map->conseil[i]);
		free(map->conseil[i]);
	}
	for(i=0;i<map->nbLines;i++){
		for(j=0;j<map->nbCol;j++){
			free(map->tab[i][j]);
		}
	}
}
