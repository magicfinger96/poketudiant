#ifndef TEAM_H
#define TEAM_H

#define MAXTEAM 3  /* max number of poketudiants in the team of the player */
#define MAXCHAR1 20 

typedef struct Team Team; 
struct Team{
  char name[MAXCHAR1];
  int pos;
  Poketudiant* pe[MAXTEAM];
};

Team* myTeamInit(Attack tabAtt[NBTYPATT]);
Team* randomTeam(int min, int max, Attack tabAtt[NBTYPATT]);

void addInTeam(Team *t, Poketudiant *pe1);
Poketudiant* in_team(Team *team, int id);
Poketudiant* supp_pe_team(Team *team, int id);

void nurse_team(Team *t);

void display_team(Team *t);
void free_team(Team *t);

#endif
