/* --- pokecafet.c --- */
/*  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "enumtochar.h"
#include "team.h"
#include "pokecafet.h"

Pokecafeteriat *initPokecafet(int nbTables,int nbChairs){
  /* return an empty pokecafeteriat */
  Pokecafeteriat *pokecafet = (Pokecafeteriat*)malloc(sizeof(Pokecafeteriat));
  pokecafet -> nbTables = nbTables;
  pokecafet -> nbChairs = nbChairs;
  int i,*pos;
  pos = (int*)malloc(sizeof(int)*nbTables);

  Poketudiant*** seat = (Poketudiant***)malloc(nbTables*sizeof(Poketudiant**));

  for(i=0;i<nbTables;i++){
    seat[i] = (Poketudiant**)malloc(nbChairs*sizeof(Poketudiant*));
    pos[i] =0;
  }

  pokecafet->pos= pos;
  pokecafet->seat = seat;

  return pokecafet;
}

int put_pe_in_cafet(Pokecafeteriat *pokecafet, Poketudiant *pe){
  /* we place pe where there is place in and we return the table
	where pe is put, if there's no place anymore: returns 0 */
  int i;
  for(i=0;i<pokecafet->nbTables;i++){
    if (pokecafet->pos[i] < pokecafet->nbChairs){
      pokecafet->seat[i][pokecafet->pos[i]++] = pe;
      return i;
    }
  }
  return -1;
}

void put_pe_table(Pokecafeteriat *pokecafet,Poketudiant *pe,int nTable){
  /* we place pe at the table nTable in the pokecafet */
  if (nTable >= 0 && nTable < pokecafet->nbTables){
    if (pokecafet->pos[nTable] < pokecafet->nbChairs){
      pokecafet->seat[nTable][pokecafet->pos[nTable]++] = pe;
    }
  }
}

void drop(Pokecafeteriat *cafet,Team *team,int id){
  /* move the poketudiant id from team to the pokecafeteriat */
  Poketudiant *pe1;
  int ntable;
	if (id == 0){
		printf("You can't move your teacher out of the team !\n");
		return;
	}
  pe1=in_team(team,id);
  if (pe1 != NULL){
    ntable = put_pe_in_cafet(cafet,pe1);
    if(ntable != -1 ){
      printf("You sent your poketudiant n°%d to the pokecafeteriat, table n°%d\n",id,ntable+1);
      supp_pe_team(team,id);
    } else {
      printf("The pokecafeteriat was full, your poketudiant n°%d ran away !!\n",id);
      supp_pe_team(team,id);
      free(pe1);
    }
  } else {
    printf("Undefined id\n");
  }
}

void move_table_req(Pokecafeteriat *cafet,Team *team,int id,int ntable){
  /* place the poketudiant id in the table n° ntable */
  Poketudiant *pe1;
  int inTeam=0; /* = 1 if pe is in the team, =0 if pe is in the cafet */
	if(id == 0)
	{
		printf("You can't move your teacher from your team !\n");
		return;
	}
  pe1=in_cafet(cafet,id);
  if (pe1 == NULL){
    pe1 = in_team(team,id);
    inTeam = 1;
  }
  if (pe1 == NULL){
    printf("The poketudiant with this id doesn't exist !\n");

    /* if the poketudiant with id exists :*/
  } else{
    /* if its change of place is achieved: */
    if (not_full_table(cafet,ntable)){
      if (inTeam){
				supp_pe_team(team,pe1->id);
      } else {
				supp_pe_cafet(cafet,pe1);
      }
      put_pe_table(cafet,pe1,ntable);
      printf("We put the poketudiant n°%d at the table n°%d!\n", pe1->id,ntable+1);
    } else {
      printf("There is no place at this table !\n");
    }
  }
}

void switch_req(Pokecafeteriat *cafet,Team *team,int id1, int id2){
  /* inverse the poketudiant id1 with id2 (in pokecafetariat or team */
  Poketudiant *p1,*p2;

  p1=in_cafet(cafet,id1);
  p2=in_cafet(cafet,id2);

	if ((p1 != NULL && id2 == 0) || (p2 != NULL && id1 == 0 ) ){
		printf("You can't put your teacher in the pokecafeteriat !\n");
		return;
	}

  if (p1 == NULL){
    p1 = in_team(team,id1);
  }
  if (p2 == NULL){
    p2 = in_team(team,id2);
  }
  if(p1 != NULL && p2 != NULL){
    exchange(p1,p2);
    printf("They've been switched !\n");
  } else {
    printf("Unknown identifiant !\n");
  }
}

void nurse_cafet(Pokecafeteriat *pokecafet){
  /* heal all my poketudiants in the pokecafeteriat */
  int i,j;
  if (pokecafet != NULL && pokecafet->nbTables && pokecafet->nbChairs){
    for(i=0;i<pokecafet->nbTables;i++){
      for(j=0;j<pokecafet->pos[i];j++){
				pokecafet->seat[i][j]->HP = pokecafet->seat[i][j]->maxHP;
      }
    }
  }
}

void supp_pe_table(Pokecafeteriat *pokecafet,int nTable,int ind){
  /* suppress the poketudiant at the ind of the table */
  int i=0;
  while(i+ind <pokecafet->pos[nTable]-1){
    pokecafet->seat[nTable][ind+i] = pokecafet->seat[nTable][ind+i+1];
    i++;
  }

  pokecafet->pos[nTable]--;
}

void supp_pe_cafet(Pokecafeteriat *pokecafet,Poketudiant *pe){
  /* suppress the poketudiant id of the pokecafeteriat and return it*/
  int i,ind;
  for(i=0;i<pokecafet->nbTables;i++){
    ind = ind_pe_cafet(pokecafet->seat[i],pokecafet->pos[i],pe->id);
    if(ind != -1){
      supp_pe_table(pokecafet,i,ind);
    }
  }
}

void pick(Pokecafeteriat *cafet,Team *team,int id){
  /* move the poketudiant n°id from the pokecafet to the team */
  Poketudiant *pe1;
  pe1=in_cafet(cafet,id);
  if(pe1 != NULL){
    if(team->pos < MAXTEAM){
      printf("You add poketudiant n°%d to your team !\n",id);
      addInTeam(team,pe1);
      supp_pe_cafet(cafet,pe1);
    } else {
      printf("You can't add it, your team is full !\n");
    }
  } else {
    printf("Undefined id !\n");
  }
}

void release(Pokecafeteriat *cafet,int id){
  /* release the poketudiant n°id from the pokecafet */
  Poketudiant *pe1;
  pe1=in_cafet(cafet,id);
  if(pe1 != NULL){
		printf("You let go you're poketudiant n°%d, you will never see it again",id);
	  supp_pe_cafet(cafet,pe1);
	  free(pe1);
  } else {
    printf("Undefined id in the pokecafeteriat !\n");
  }
}

int empty_cafet(Pokecafeteriat *cafet){
  /* returns 1 if the cafet is empty */
  int i;
  for(i=0;i<cafet->nbTables;i++){
    if (cafet->pos[i] != 0){
      return 0;
    }
  }
  return 1;
}

int success_capture(Team *myTeam,Pokecafeteriat* pokecafet,Poketudiant *wildPe){
  /* handles when the capture of the wild pe is achieved, returns 1 if it's done */
  int room;
  printf("You caught %s n°%d !\n",varietytochar(wildPe->variety),wildPe->id);

  if (myTeam->pos < MAXTEAM){
    /* wildPe is added to the team: */

    addInTeam(myTeam,wildPe);
    printf("We put it on your team.\n");
  } else {
    /* wildPe is put in the Pokecafeteriat */
    room = put_pe_in_cafet(pokecafet,wildPe);

    if(room != -1){
      printf("We put it on your pokecafetariat, table n°%d\n",room+1);
    } else {
      printf("There's no place anymore, It ran away\n");
      //free(wildPe);
      return 0;
    }
  }
  return 1;
}

int not_full_table(Pokecafeteriat *pokecafet,int nTable){
  /* return 1 if table n° nTable is full else 0 */
  return pokecafet->pos[nTable] < pokecafet->nbChairs;
}

Poketudiant* in_cafet(Pokecafeteriat *pokecafet,int id){
  /* return poketudiant id if it's in pokecafet else NULL */
  int i,j;
  for(i=0;i<pokecafet->nbTables;i++){
    j = 0;
    while(j< pokecafet->pos[i]){
      if(pokecafet->seat[i][j]->id == id){
	return pokecafet->seat[i][j];
      }
      j++;
    }
  }
  return NULL;
}

int ind_pe_cafet(Poketudiant **table,int nbPe,int id){
  /* return indice of the pe with id in table or -1 if not here */
  int i = 0;
  while(i< nbPe){
    if (table[i]->id == id){
      return i;
    }
    i++;
  }
  return -1;
}

void show_pokecafet(Pokecafeteriat *pokecafet){
  /* show the pokecafeteria */
  int i,j;
  if (pokecafet != NULL && pokecafet->nbTables && pokecafet->nbChairs){
    printf("There are %d table(s) of %d chair(s).\n\n",
	   pokecafet->nbTables,pokecafet->nbChairs);

    for(i=0;i<pokecafet->nbTables;i++){
      printf("Table n°%d:\n",i+1);
      for(j=0;j<pokecafet->pos[i];j++){
				displayPE(pokecafet->seat[i][j]);
      }
    }

  } else {
    printf("The pokecateriat is empty !\n");
  }
}

void show_table(Pokecafeteriat *pokecafet,int ntable){
  /* show the table n° ntable+1 */
  int j;
  if (pokecafet != NULL && pokecafet->nbTables && pokecafet->nbChairs){
    if (ntable >= 0 && ntable < pokecafet->nbTables){
      printf("At the table n°%d, there are:\n",ntable+1);
      for(j=0;j<pokecafet->pos[ntable];j++){
				displayPE(pokecafet->seat[ntable][j]);
      }
    }
  } else {
    printf("The pokecafeteriat is empty !\n");
  }
}

void free_pokecafet(Pokecafeteriat *pokecafet){
  /* free the pokecafeteriat */
  int i,j;
  for(i=0;i<pokecafet->nbTables;i++){
    for(j=0;j<pokecafet->pos[i];j++){
      free(pokecafet->seat[i][j]);
    }
  }

  for(i=0;i<pokecafet->nbTables;i++){
    Poketudiant** current = pokecafet->seat[i];
    free(current);
  }
  free(pokecafet->pos);
  free(pokecafet->seat);
}
