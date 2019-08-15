/* ----- team.c ----- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "team.h"

Team* myTeamInit(Attack tabAtt[NBTYPATT]){
  /* initialize my team */
  Team *t = (Team*)malloc(sizeof(Team));
  Poketudiant *pe0 = randomPE(1,TEACHER,tabAtt);
	pe0->XP=0;
  strcpy(t->name,"My team");
  t->pos = 1;
  t->pe[0]=pe0;
  return t;
}

Team* randomTeam(int min,int max,Attack tabAtt[NBTYPATT]){
  /* generates a full team of poketudiants with lvl in [min,max] */
  Team *t = (Team*)malloc(sizeof(Team));
  int i,lvl;
  t->pos = 3;
  strcpy(t->name,"Rival team");

  lvl=rand()%(max-min+1)+min;
  t->pe[0]= randomPE(lvl,TEACHER,tabAtt);

  for(i=1;i<MAXTEAM;i++){
    lvl=rand()%(max-min+1)+min;
    t->pe[i]= randomPE(lvl,ALLPE,tabAtt);
  }

  return t;
}

void addInTeam(Team *t,Poketudiant *pe1){
  /* adds a poketudiant to the team */
  if(t->pos < MAXTEAM){
    t->pe[t->pos++] = pe1;
  } else {
    printf("Well, It seems like the team is already filled.\n");
  }
}

Poketudiant* in_team(Team *team,int id){
  /* return the poketudiant id of the team else NULL */
  int i=0;
  while(i< team->pos){
    if (team->pe[i]->id == id){
      return team->pe[i];
    }
    i++;
  }
  return NULL;
}

Poketudiant* supp_pe_team(Team *team,int id){
  /* suppress the poketudiant id of the team and return it*/
  Poketudiant *supp;
  int i=0;
  while(i< team->pos && team->pe[i]->id != id) i++;
  if(i <team->pos){
    supp = team->pe[i];
    while(i <team->pos-1){
      team->pe[i] = team->pe[i+1];
      i++;
    }
    team->pos--;
    return supp;
  }
  return NULL;
}

void nurse_team(Team *t){
  /* heal all my poketudiants in the team */
  int i=0;
  if (t->pos != 0){
    while(i < t->pos){
      t->pe[i]->HP = t->pe[i]->maxHP;
      i++;
    }
  }
}

void display_team(Team *t){
  /* display the team */
  int i=0;
  if (t->pos == 0){ printf("The team is empty !\n"); return;}
  printf("%s :\n",t->name);
  while(i < t->pos){
    displayPE(t->pe[i++]);
  }
  printf("\n");
}

void free_team(Team *t){
  /* free the team */
  int i;
  for(i=0;i< t->pos;i++){
    free(t->pe[i]);
  }
}
