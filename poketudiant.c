/* ----- poketudiant.c ----- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "enumtochar.h"

const Variety varieties[NBVAR] = VARIETIES;
const Type types[NBVAR] = TYPES;
const int capturables[NBVAR]= CAPTURABLES;
const int attacks[NBVAR] = ATTACKS;
const int defenses[NBVAR] = DEFENSES;
const int maxHPs[NBVAR] = MAXHPS;
const AttName attNames[NBTYPATT]= ATTNAMES;

const Variety evols[NBVAR] = EVOLS;

float random_float(float min,float max){
  /* return a random float in [min,max] */
  float k0,k;
  int minD,maxD;
  min*=1000; max*=1000;
  minD = min; maxD = max;
  k0 = rand()%(minD-maxD+1)+minD;
  k = k0/1000.0;
  return k;
}

int newMaxXP(Poketudiant *pe){
  /* returns the new maxXp of pe */
  return COEFXP*((1.0+pe->lvl)/2.0);
}

void lvlUp(Poketudiant *pe, int xp);

Poketudiant* randomPE(int lvl, int nature, Attack tab[NBTYPATT]){
  /* creates a random poketudiant (wild, dressed or teacher) and returns it*/
  Poketudiant *pe = (Poketudiant*)malloc(sizeof(Poketudiant));
  int x,x2,x3,choices[6],i,j=0,prop[NBVARWILD] = INDWILD;
  float coef[3];

  switch(nature){
  case WILD:
    x = prop[rand()%NBVARWILD];
    break;
  case TEACHER:
    x = EDRESSEUR;
    break;
  default:
    do{
      x = rand()%NBVAR;
    }while(x==EDRESSEUR);
    break;
  }
  for (i=0;i<3;i++) coef[i] = random_float(MINCOEF,MAXCOEF) ;

  pe->nickname[0] = 0;
  pe->id =id++;
  pe->variety = varieties[x];
  pe->type = types[x];
  pe->capturable = capturables[x];
  pe->attPt = attacks[x]*coef[0];
  pe->defPt = defenses[x]*coef[1];
  pe->maxHP = maxHPs[x]*coef[2];
  pe->HP = pe->maxHP;
  pe->lvl = 1;
  pe->maxXP = COEFXP;
  pe->XP = rand()%pe->maxXP;

  /* in order to not make the poketudiant evolve if it has lvl >5: */
  if(nature == WILD){
    pe->evol = TMPEM;
  } else {
			pe->evol = evols[x];
  }

  /* saves the coef of the stats, useful when it passes the next lvl */
  for(i=0;i<3;i++){
    pe->coefStats[i] = coef[i];
  }

  /* x saves the indice of the first variety in varieties[] with the 
	same type of pe's */
  switch(pe->type){
  case NOISY:
    x3 = PARLFOR;
    break;
  case LAZY:
    x3 = PROCRASTINO;
    break;
  case MOTIVATED:
    x3 = BUCHAFON;
    break;
  case TEACHER:
    x3 = EDRESSEUR;
    break;
  default:
    printf("error");
    break;
  }
  x2 = rand()%NBPOW;
  pe->attacks[0]= tab[x3+x2];

  /* we put all the potentialy good indices of the tab[] 
	(for the 2nd attack) in choices[] */
  for(i=0;i<9 && j<6;i++){
    if(pe->type == tab[i].type){
      i+=3;
    }
    choices[j]=i;
    j++;
  }

  x2 = rand()%6;
  pe->attacks[1]= tab[choices[x2]];

  /* put the poketudiant at its defined level */
  for (i=1;i<lvl && i<LVLMAX;i++){
    lvlUp(pe,pe->XP);
  }

  if(pe->evol == TMPEM) pe->evol = evols[x];

  return pe;
}

Poketudiant* specificPE(int lvl,Variety variety, Attack tab[NBTYPATT]){
  /* creates a random dressed poketudiant with a specific lvl and variety */
  Poketudiant *pe = (Poketudiant*)malloc(sizeof(Poketudiant));
  int x2,x3,choices[6],i,j=0;
  float coef[3];

  for (i=0;i<3;i++) coef[i] = random_float(MINCOEF,MAXCOEF);

  pe->nickname[0] = 0;
  pe->id =id++;
  pe->variety = varieties[variety];
  pe->type = types[variety];
  pe->capturable = capturables[variety];
  pe->attPt = attacks[variety]*coef[0];
  pe->defPt = defenses[variety]*coef[1];
  pe->maxHP = maxHPs[variety]*coef[2];
  pe->HP = pe->maxHP;
  pe->lvl = 1;
  pe->maxXP = COEFXP;
  pe->XP = rand()%pe->maxXP;
  pe->evol = evols[variety];

  /* saves the coef of the stats, useful when it passes the next lvl */
  for(i=0;i<3;i++){
    pe->coefStats[i] = coef[i];
  }

  /* x saves the indice of the first variety in varieties[] with the 
	same type of pe's */
  switch(pe->type){
  case NOISY:
    x3 = PARLFOR;
    break;
  case LAZY:
    x3 = PROCRASTINO;
    break;
  case MOTIVATED:
    x3 = BUCHAFON;
    break;
  case TEACHER:
    x3 = EDRESSEUR;
    break;
  default:
    printf("error");
    break;
  }
  x2 = rand()%NBPOW;
  pe->attacks[0]= tab[x3+x2];

  /* we put all the potentialy good indices of the tab[] 
	(for the 2nd attack) in choices[] */
  for(i=0;i<9 && j<6;i++){
    if(pe->type == tab[i].type){
      i+=3;
    }
    choices[j]=i;
    j++;
  }

  x2 = rand()%6;
  pe->attacks[1]= tab[choices[x2]];

  /* put the poketudiant at its defined level */
  for (i=1;i<lvl && i<LVLMAX;i++){
    lvlUp(pe,pe->XP);
  }

  return pe;
}

void evolve(Poketudiant *pe){
  /* evolves the poketudiant pe */
  if(pe->evol == EMPTY) return;
  float coef1 = 1+(0.1*pe->lvl);
  pe->attPt = attacks[pe->evol]*coef1*pe->coefStats[0];
  pe-> defPt = defenses[pe->evol]*coef1*pe->coefStats[1];
  pe-> maxHP = (float)(maxHPs[pe->evol])*coef1*pe->coefStats[2];
  pe->HP = pe-> maxHP;
  pe->capturable = 0;
  pe->variety = pe->evol;
  pe->evol = EMPTY;
}

void exchange(Poketudiant *a,Poketudiant *b){
  /* switch poketudiant a with b */
  Poketudiant tmp;
  tmp=*a;
  *a=*b;
  *b=tmp;
}

void lvlUp(Poketudiant *pe, int xp){
  /* increases the level of the poketudiant pe and add its xp */
  int x;
  if (pe->lvl >= LVLMAX){
    pe->XP = pe->maxXP;
    return;
  }
  pe->lvl+=1;
  pe->XP = xp;
  pe->maxXP = newMaxXP(pe);
  pe->attPt += attacks[pe->variety]*pe->coefStats[0]*0.1;
  pe->defPt += defenses[pe->variety]*pe->coefStats[1]*0.1;
  pe->maxHP += maxHPs[pe->variety]*pe->coefStats[2]*0.1;
  pe->HP = pe->maxHP;
  if (pe->evol != EMPTY && pe->evol != TMPEM){
    /* triggers the evolution or not */
    switch(pe->lvl){
    case 3:
      x= rand()%5; /* 20% <==> 1/5 */
      if (!x) evolve(pe);
      break;
    case 4:
      x= rand()%1000; /* 37.5% <==> 375/1000 */
      if (x<=374) evolve(pe);
      break;
    default:
      if(pe->lvl>=5) evolve(pe);
      break;
    }
  }
  if (pe->XP>pe->maxXP)lvlUp(pe,pe->XP-pe->maxXP);
}

int totalXP(Poketudiant *pe){
  /* return the total number of XP points earned by pe since niv1 */
  int i, nb=0;
  for(i=1;i<pe->lvl;i++){
    nb+=COEFXP*((1+i)/2);
  }
  return nb+=pe->XP;
}

int earnXP(Poketudiant *pe1, Poketudiant *pe2, int nbPE){
  /* increases the xp points of pe1 which beat pe2 with nbPE-1 other pe 
	and returns the number of XP earned */
  float t;
  if (pe1->lvl < LVLMAX){
    t= totalXP(pe2)*0.1;
    pe1->XP+=t/nbPE;
    if(pe1->XP >= pe1->maxXP){
      lvlUp(pe1,pe1->XP-pe1->maxXP);
    }
  }
  return t/nbPE;
}

void earnXP_debog(Poketudiant *pe1,int xp){
  /* function for debogging: add xp to pe1 */
  if (pe1->lvl < LVLMAX){
    pe1->XP=xp;
    if(pe1->XP >= pe1->maxXP){
      lvlUp(pe1,pe1->XP-pe1->maxXP);
    }
  }
}

void fill_att(Attack tab[NBTYPATT]){
  /* fills the table of all the existing attacks */
  int i, powers[NBTYPATT] = POWLIST, attypes[NBTYPATT] = ATTYPES;

  for(i=0;i<NBTYPATT;i++){
    tab[i].name = attNames[i];
    tab[i].type = attypes[i];
    tab[i].power = powers[i];
  }
}

void displayAtt(Attack list[NBVAR]){
  /* display all the struct Attack created for the game */
  int i;
  printf("The attacks:\n");
  for(i=0;i<NBTYPATT;i++){
    printf("	%s	(%s)	%d\n",attacktochar(list[i].name),
	   typetochar(list[i].type),list[i].power);
  } 
}

void full_display(Poketudiant *pe){
  /* display a poketudiant pe with its caracteristics */
  int i;
  printf("Poketudiant n°%d\n	variety: %s\n	type: %s\n\
	level: %d\n	XP: %d/%d\n	capturable: %s\n	attack points: %d\n\
	defense points: %d\n	HP: %d/%d\n",pe->id,varietytochar(pe->variety),
	 typetochar(pe->type),pe->lvl,pe->XP,pe->maxXP,pe->capturable?"yes":"no",
	 pe->attPt,pe->defPt,pe->HP,pe->maxHP);

  if (pe->evol != EMPTY)printf("  evolution: %s\n",varietytochar(pe->evol));

  for (i=0;i<2;i++){
    printf("	attack %d: %s (%s) %d\n",i+1,attacktochar(pe->attacks[i].name),
	   typetochar(pe->attacks[i].type),pe->attacks[i].power);
  }
  printf("\n");

  if (strcmp(pe->nickname,"")){
    printf("  nickname: %s\n ",pe->nickname);
  }
}

void displayPE(Poketudiant *pe){
  /* display a poketudiant pe with its caracteristics */

  printf("	Poketudiant n°%d: %s(lvl %d) HP: %d/%d  ",
  pe->id,varietytochar(pe->variety), pe->lvl,pe->HP,pe->maxHP);
  printf("\n");
}
