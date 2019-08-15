#ifndef FIGHTMAIN_H
#define FIGHTMAIN_H

#define MAXI(a,b) (a>=b?a:b)
#define MINI(a,b) (a<=b?a:b)
#define ABS(a) (a>0?a:-a)


//define in fight.c
int attack(Poketudiant *pe1, Poketudiant *pe2, int nAtt);
int fled(Poketudiant *pe1, Poketudiant *pe2);
int capture(Team *myTeam,Poketudiant *wildPE);
void wildDisplay(Poketudiant *pe);
void rivalDisplay(Poketudiant *pe);

//define in wildMain.c
void emptyBuffer(void);
void wild(Team* myTeam, Pokecafeteriat* pokecafet, int min, int max,Attack tabAtt[NBTYPATT]);

//define in rivalMain.c
void rival(Team* myTeam,Team* rivalTeam, int min, int max, Attack tbAtt[NBTYPATT]);

#endif
