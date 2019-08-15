#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "enumtochar.h"
#include "team.h"
#include "pokecafet.h"
#include "fightMain.h"
#include "map.h"
#include "request.h"

int id=0;

int main() {
  srand(time(NULL));
	FILE* fichier = fopen("map.txt","r+");
  Team *myTeam;
	Pokecafeteriat *pokecafet;
	Map *map;

  /* creates table of all attacks */
  Attack tabAtt[NBTYPATT];
  fill_att(tabAtt);
  //displayAtt(tabAtt);

	if(fichier != NULL){
		/* initialize pokecafeteriat et my team */
		pokecafet = initPokecafet(NBTABLES,NBCHAIRS);
		myTeam = myTeamInit(tabAtt);

		/* initialize map and the "conseil des diplomes" */
		map=createMap(fichier,tabAtt);


		/* launch the command traitment */
		request(myTeam,pokecafet,tabAtt,map);

		/* free all the malloc */
		free_pokecafet(pokecafet);
		free(pokecafet);
		free_team(myTeam);
		free(myTeam);
		freeMap(map);
		free(map);
		fclose(fichier);

	} else {
		printf("Error (file)\n");
	}

  return 0;
}
