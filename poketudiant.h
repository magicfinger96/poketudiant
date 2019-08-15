#ifndef POKETUDIANT_H
#define POKETUDIANT_H

#define ERROR "Erreur"

#define NBVAR	10  /* quantity of varieties*/
#define NBVARWILD 6	/* quantity of varieties that can be found in nature*/
#define NBTYPATT 12 /*number of attack types */
#define NBATT 2 /* number of attacks for each poketudiants */
#define NBPOW 3 /* number of "power" differents */
#define EMPTY -2 /* used in evol if the pokemon doesn't have evolution */
#define TMPEM -1  /* used in evol temporarly if the pokemon is wild */ 
#define MINCOEF 0.9 /* minimal coefficient of the random stats*/
#define MAXCOEF 1.1 /* maximal coefficient of the random stats*/
#define WILD 0   /* represents wild poketudiant */
#define ALLPE 1  /* represents all poketudiant natures (WILD,DRESSED) */
#define LVLMAX 10 /* represents the max level of any poketudiant */
#define COEFXP 500 /* coefficient in the formule to calculate XP earned */

#define INDWILD {PARLFOR,ISMAR,PROCRASTINO,COUCHTAR,BUCHAFON,BELMENTION} /* potential wild ones in the varieties list */
#define CAPTURABLES {1,1,0,1,1,0,1,1,0,0} /* capturable?1:0 for VARIETY */
#define ATTACKS {60,50,85,40,30,55,50,30,70,100} /* stats attack for VARIETY */
#define DEFENSES {40,30,55,60,50,85,50,50,70,100} /* stats defense for VARIETY */
#define	MAXHPS {60,40,70,60,40,70,60,40,70,100} /* max health point for VARIETY */
#define EVOLS {EMPTY,RIGOLAMOR,EMPTY,EMPTY,NUIDEBOU,EMPTY,EMPTY,PROMOMAJOR,EMPTY,EMPTY} /* evolutions for VARIETY */
#define TYPES {NOISY,NOISY,NOISY,LAZY,LAZY,LAZY,MOTIVATED,MOTIVATED,MOTIVATED,TEACHER} /* type for VARIETY */
#define VARIETIES {PARLFOR,ISMAR,RIGOLAMOR,PROCRASTINO,COUCHTAR,NUIDEBOU,BUCHAFON,BELMENTION,PROMOMAJOR,EDRESSEUR} /* name of VARIETY */

#define ATTNAMES {BAVARDAGE,GROBOUCAN,MEGAV,BAILLEMENT,PTISOM,SDODO,OBOULO,EXOMAI,MAXREVIZ,TITQ,POSERCO,FATALINT} /* name of ATTACK */
#define POWLIST {10,15,20,10,15,20,10,15,20,10,15,20} /* power of ATTACK */
#define ATTYPES {NOISY,NOISY,NOISY,LAZY,LAZY,LAZY,MOTIVATED,MOTIVATED,MOTIVATED,TEACHER,TEACHER,TEACHER} /* types for ATTACK */

extern int id;

typedef enum {NOISY,LAZY,MOTIVATED,TEACHER} Type;
typedef enum {PARLFOR,ISMAR,RIGOLAMOR,PROCRASTINO,COUCHTAR,NUIDEBOU,BUCHAFON,BELMENTION,PROMOMAJOR,EDRESSEUR} Variety;
typedef enum {BAVARDAGE,GROBOUCAN,MEGAV,BAILLEMENT,PTISOM,SDODO,OBOULO,EXOMAI,MAXREVIZ,TITQ,POSERCO,FATALINT,PANORDI} AttName;


typedef struct Attack Attack; 
struct Attack{
  AttName name;
  Type type;
  int power;
};

typedef struct Poketudiant Poketudiant; 
struct Poketudiant{
  int id;
  char nickname[13];
  Variety variety;
  Type type;
  int capturable;
  int attPt;
  Attack attacks[NBATT];
  int defPt;
  int maxHP;
  int maxXP;
  int XP;
  int HP;
  int lvl;
  Variety evol;
  float coefStats[3]; /* contains the coef of the starting stats (attPt,defPt,maxHP) */
};


float random_float(float min, float max);
int newMaxXP(Poketudiant *pe);

Poketudiant* randomPE(int lvl, int nature, Attack tab[NBTYPATT]);
Poketudiant* specificPE(int lvl,Variety variety, Attack tab[NBTYPATT]);
void exchange(Poketudiant *a, Poketudiant *b);

void evolve(Poketudiant *pe);
void lvlUp(Poketudiant *pe, int xp);
int totalXP(Poketudiant *pe);
int earnXP(Poketudiant *pe1, Poketudiant *pe2, int nbPE);
void earnXP_debog(Poketudiant *pe1, int xp);

void fill_att(Attack tab[NBTYPATT]);

void displayAtt(Attack list[NBVAR]);
void displayPE(Poketudiant *pe);
void full_display(Poketudiant *pe);

#endif
