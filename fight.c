/* ---- fight.c ---- */
/* All the functions about the rival and wild fight */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "enumtochar.h"
#include "team.h"
#include "pokecafet.h"
#include "fightMain.h"

int attack(Poketudiant *pe1, Poketudiant *pe2, int nAtt){
  /* the poketudiant pe1 attacks pe2 with its attack n°nAtt
	and returns the number of HP lost by pe2 */
  float pow = pe1->attacks[nAtt].power,k0,k;
  int dmg;
  if ((pe1->type+1)%3 == pe2->type || (pe1->type == TEACHER && pe2->type != TEACHER)) pow+=pow;
  k0 = rand()%(1100-900+1)+900;
  k = k0/1000.0;
  k0 = (float)pe1->attPt/(float)pe2->defPt;
  dmg = k*k0*pow;
  if (pe2->HP-dmg < 0) dmg = pe2->HP;
  pe2->HP-=dmg;
  return dmg;
}

int fled(Poketudiant *pe1,Poketudiant *pe2){
  /* returns 1 if pe1 managed to flee, otherwise 0 */
  int x= rand()%100;
  switch(ABS(pe1->lvl-pe2->lvl)){
  case 0:
    return x<50;
  case 1:
    if (pe1->lvl > pe2->lvl) return x<75;
    return x<40;
  case 2:
    if (pe1->lvl > pe2->lvl) return x<90;
    return x<25;
  default:
    return pe1->lvl > pe2->lvl;
  }
}

int capture(Team *myTeam,Poketudiant *wildPE){
  /* myTeam tries to capture wildPE, returns 1 if it's a success else 0 */
  float prob = 2*MAXI(0.5-(wildPE->HP/wildPE->maxHP),0)*100;
  return (rand()%100) < prob;
}

void wildDisplay(Poketudiant *pe){
  /* Display the choices for the player during a wild fight */
  printf("Pick an action:\n	1. Use the attack %s \n\
	2. Use the attack %s \n	3. Switch your Poketudiant\n	4. Try the\
 catch\n	5. Try to flee\n\n",attacktochar(pe->attacks[0].name),
	 attacktochar(pe->attacks[1].name));
}

void rivalDisplay(Poketudiant *pe){
  /* Display the choices for the player during a wild fight */
  printf("Pick an action:\n	1. Use the attack %s\n	2. Use\
 the attack %s\n	3. Switch your Poketudiant\n\n",
	 attacktochar(pe->attacks[0].name),attacktochar(pe->attacks[1].name));
}
